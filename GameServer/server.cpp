#include "server.h"
#include <assert.h>
#include <memory.h>
#include <iostream>
#include <ctime>

pthread_mutex_t Server::mutex = PTHREAD_MUTEX_INITIALIZER;

Server::Server() {
  m_serverSock = -1;
  m_clientSock[0] = -1;
  m_clientSock[1] = -1;
  m_userNames[0] = NULL;
  m_userNames[1] = NULL;
  memset(&m_clientAddr[0], 0, sizeof(m_clientAddr[0]));
  memset(&m_clientAddr[1], 0, sizeof(m_clientAddr[1]));
  pthread_mutex_init(&mutex, NULL);
}

Server::~Server() {
  if (m_serverSock >= 0) {
    close(m_serverSock);
  }
  for (int i = 0; i < 2; i++) {
    if (m_clientSock[i] >= 0) {
      close(m_clientSock[i]);
    }
  }
}

void Server::SetServer(const char* IPAddr, int port) {
  memset(&m_serverAddr, 0, sizeof(m_serverAddr));
  m_serverAddr.sin_family = AF_INET;
  m_serverAddr.sin_addr.s_addr = inet_addr(IPAddr);
  m_serverAddr.sin_port = htons(port);

  /*Get Socket*/
  m_serverSock = socket(AF_INET, SOCK_STREAM, 0);
  if (m_serverSock < 0) {
    std::cout << "Socket Error" << std::endl;
    return;
  }

  /*Bind and Listen*/
  if (bind(m_serverSock, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) < 0) {
    std::cout << "Bind Failed" << std::endl;
    return;
  }
  if (listen(m_serverSock, 20)) {
    std::cout << "Listen Failed" << std::endl;
    return;
  }

  std::cout << "Server Complete" << std::endl;
}

void Server::RunServer() {
  if (m_serverSock < 0) {
    std::cout << "Please Set Server" << std::endl;
    return;
  }

  pthread_t threadState, threadRequest[2];
  ThreadVar1 var1 = { m_clientSock, m_time, m_tank };
  if ( pthread_create( &threadState, NULL, TankState, &var1) < 0 ) {
    std::cout << "State Thread Error" << std::endl;
  }

  while (true) {
    //waiting for connection
    sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);
    int clientSock = accept(m_serverSock, (sockaddr*)(&clientAddr), &clientSize);
    if (clientSock < 0) {
      std::cout << "Accept Error" << std::endl;
      break;
    }
    if (m_clientSock[0] < 0 || m_clientSock[1] < 0) {
      char messageBuffer[100] = "";
      read( clientSock, messageBuffer, sizeof(messageBuffer) );
      int* tp = (int*)(messageBuffer + 1);
      int nameSize = *tp;
      char* userName = new char[nameSize];
      memcpy(userName, messageBuffer + 5, nameSize);

      if (m_clientSock[0] < 0) {
        m_clientSock[0] = clientSock;
        m_clientAddr[0] = clientAddr;
        if (m_userNames[0]) delete m_userNames[0];
        m_userNames[0] = new char[nameSize + 1];
        memcpy(m_userNames[0], messageBuffer + 5, nameSize);
        m_userNames[0][nameSize] = '\0';
        ThreadVar2 var2 = {m_userNames, m_clientSock, 0, m_tank };
        if ( pthread_create( &threadRequest[0], NULL, GetRequest, &var2) < 0 ) {
          std::cout << "Request1 Thread Error" << std::endl;
        }
      }
      else { //m_clientSock[1] < 0
        m_clientSock[1] = clientSock;
        m_clientAddr[1] = clientAddr;
        if (m_userNames[1]) delete m_userNames[1];
        m_userNames[1] = new char[nameSize + 1];
        memcpy(m_userNames[1], messageBuffer + 5, nameSize);
        m_userNames[1][nameSize] = '\0';
        ThreadVar2 var2 = {m_userNames, m_clientSock, 1, m_tank };
        if ( pthread_create( &threadRequest[1], NULL, GetRequest, &var2) < 0) {
          std::cout << "Request2 Thread Error" << std::endl;
        }
      }

      //Annonuce to every client
      for (int i = 0; i < 2; i++) {
        if (m_clientSock[i] >= 0) {
          const char* join = " joined the game.";
          int nameSize = strlen(userName);
          int joinSize = strlen(join);
          int bufferSize = 4 + 4 + nameSize + joinSize + 1;
          char* joinBuffer = new char[bufferSize];
          ((int*)joinBuffer)[0] = bufferSize - 4;
          ((int*)joinBuffer)[1] = 1;
          memcpy(joinBuffer + 8, userName, nameSize);
          memcpy(joinBuffer + 8 + nameSize, join, joinSize);
          joinBuffer[bufferSize - 1] = '\0';
          write(m_clientSock[i], joinBuffer, bufferSize);
          delete[] joinBuffer;
        }
      }

      delete[] userName;
    }
    else {
      std::cout << "Can Not Join" << std::endl;
    }
  }
}

void* Server::TankState(void* var) {
  ThreadVar1* tvp = (ThreadVar1*)var;
  pthread_detach(pthread_self());
  int* socks = tvp -> socks;
  time_t timeNow = tvp -> timeNow;
  Tank* tank = tvp -> tank;
  while (true) {
    time_t tempT = time(0);
    if (timeNow < tempT) {
      timeNow = tempT;

      pthread_mutex_lock(&mutex);
      for (int i = 0; i < 2; i++) {
        tank[i].Step();
      }
      pthread_mutex_unlock(&mutex);

      pthread_mutex_lock(&mutex);
      for (int i = 0; i < 2; i++) {
        if (socks[i] >= 0) {
          int bufferSize = 4 + 4 + (int)sizeof(State) + 1;
          char* stateBuffer = new char[bufferSize];
          ((int*)stateBuffer)[0] = bufferSize - 4;
          ((int*)stateBuffer)[1] = 0;
          State state = tank[i].GetState();
          memcpy(stateBuffer + 8, &state, sizeof(State));
          stateBuffer[bufferSize - 1] = '\0';
          if (socks[i] >= 0) {
            write(socks[i], stateBuffer, bufferSize);
          }
          delete[] stateBuffer;
        }
      }
      pthread_mutex_unlock(&mutex);
    }
    sleep(1);
  }
}

void* Server::GetRequest(void* var) {
  ThreadVar2* tvp = (ThreadVar2*)var;
  pthread_detach(pthread_self());
  char** userNames = tvp -> userNames;
  int* clientSocks = tvp -> clientSocks;
  int side = tvp -> side;
  Tank* tank = tvp -> tank;
  while (true) {
    char messageBuffer[100] = "";
    read( clientSocks[side], messageBuffer, sizeof(messageBuffer) );
    pthread_mutex_lock(&mutex);
    int rev = Operation(messageBuffer, userNames, side, clientSocks, tank);
    pthread_mutex_unlock(&mutex);
    if (rev < 0) {
      pthread_exit(0);
    }
  }
}

int Server::Operation(const char* message, char** userNames, int side, int* clientSock, Tank* tank) {
  int antiSide = (side == 1) ? 0 : 1;

  if ( strcmp("s", message) == 0 ) {
    //Get Shoot
    tank[antiSide].GetShot();
    std::cout << "Shoot" << std::endl;
    return 0;
  }
  else if ( strcmp("f", message) == 0 ) {
    //Camera Disturb
    tank[antiSide].DisturbCam();
    tank[side].UseDisCam();
    std::cout << "Flash" << std::endl;
    return 0;
  }
  else if ( strcmp("e", message) == 0 ) {
    //Cannon Disturb
    tank[antiSide].DisturbCannon();
    tank[side].UseDisCannon();
    std::cout << "EMP" << std::endl;
    return 0;
  }
  else if ( strcmp("o", message) == 0 ) {
    Leave(userNames, side, clientSock, tank);
    return -1;
  }
  else {
    Leave(userNames, side, clientSock, tank);
    return -1;
    // assert(0 > 1);
  }
  return -2;
}

void Server::Leave(char** userNames, int side, int* clientSock, Tank* tank) {
  clientSock[side] = -1;
  const char* leave = " left the game.";
  int bufferSize = 4 + 4 + strlen(userNames[side]) + strlen(leave) + 1;
  char* message = new char[bufferSize];
  ((int*)message)[0] = bufferSize - 4;
  ((int*)message)[1] = 1;
  memcpy(message + 8, userNames[side], strlen(userNames[side]));
  memcpy(message + 8 + strlen(userNames[side]), leave, strlen(leave));
  message[bufferSize - 1] = '\0';

  for (int i = 0; i < 2; i++) {
    if (clientSock[i] >= 0) {
      write(clientSock[i], message, bufferSize);
    }
  }
  delete[] message;
}

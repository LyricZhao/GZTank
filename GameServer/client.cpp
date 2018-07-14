#include "client.h"
#include "Tank.h"
#include <string>
#include <iostream>
#include <assert.h>
#include <pthread.h>

Client::Client() {
  m_clientSock = -1;
}

Client::Client(std::string userName) {
  m_userName = userName;
  m_clientSock = -1;
}

Client::~Client() {
  if (m_clientSock >= 0) {
    close(m_clientSock);
  }
}

void Client::ConnectToServer(const char* IPAddr, int port) {
  /*Set Address*/
  memset(&m_servAddr, 0, sizeof(m_servAddr));
  m_servAddr.sin_family = AF_INET;
  m_servAddr.sin_addr.s_addr = inet_addr(IPAddr);
  m_servAddr.sin_port = htons(port);

  m_clientSock = socket(AF_INET, SOCK_STREAM, 0);
  if ( connect(m_clientSock, (sockaddr*)(&m_servAddr), sizeof(m_servAddr)) < 0) {
    std::cout << "Connection Error" << std::endl;
    exit(1);
  }

  int nameBufferSize = 1 + 4 + (int)m_userName.size() + 1;
  char* nameBuffer = new char[nameBufferSize];
  nameBuffer[0] = 'u';
  int* tp = (int*)(nameBuffer + 1);
  *tp = nameBufferSize - 4;
  memcpy(nameBuffer + 5, m_userName.c_str(), m_userName.size());
  nameBuffer[nameBufferSize - 1] = '\0';
  write(m_clientSock, nameBuffer, nameBufferSize);

  pthread_t threadRecv;
  if ( pthread_create(&threadRecv, NULL, Receive, &m_clientSock) < 0 ) {
    std::cout << "Receive Thread Error" << std::endl;
    return;
  }
}

void* Client::Receive(void* clientSock) {
  int sock = *(int*)clientSock;
  while (true) {
    char buffer[100];
    read(sock, buffer, sizeof(buffer));
    int type = ((int*)buffer)[1];
    int size = ((int*)buffer)[0] - 4 - 1;
    if (type == 1) {
      //Annonuce
      std::cout << buffer + 8 << std::endl;
    }
    else {
      State state;
      memcpy(&state, buffer + 8, size);
      std::cout << "life: " << state.life << std::endl;
      std::cout << "EMP Waiting: " << state.cannonDisWaitingTime << std::endl;
      std::cout << "Flash Waiting: " << state.camDisWaitingTime << std::endl;
      std::cout << "Cannon Disturb: " << state.cannonDisTime << std::endl;
      std::cout << "Camera Disturb: " << state.camDisTime << std::endl;
    }
    std::cout << "Receive" << std::endl;
  }
  return NULL;
}

void Client::SendRequest() {
  while (true) {
    int x = 0;
    std::cin >> x;
    if (x == 0) {
      write(m_clientSock, "s", 1);
    }
    else if (x == 1) {
      write(m_clientSock, "f", 1);
    }
    else if (x == 2) {
      write(m_clientSock, "e", 1);
    }
  }
}

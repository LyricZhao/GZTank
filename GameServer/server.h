#ifndef SERVER_H_
#define SERVER_H_

#include "Tank.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <pthread.h>
#include <ctime>

class Server {
private:
  sockaddr_in m_serverAddr;
  int m_serverSock;

  sockaddr_in m_clientAddr[2];
  int m_clientSock[2];

  Tank m_tank[2];

  char* m_userNames[2];

  struct ThreadVar1 {
    int* socks;
    time_t timeNow;
    Tank* tank;
  };

  struct ThreadVar2 {
    char** userNames;
    int* clientSocks;
    int side;
    Tank* tank;
  };

  static pthread_mutex_t mutex;

  static void* GetRequest(void* );
  static void* TankState(void* );
  static int Operation(const char* message, char** userNames, int side, int* clientSock, Tank* tank);
  static void Leave(char** userNames, int side, int* clientSock, Tank* tank);
  time_t m_time;

public:
  Server();
  ~Server();
  void SetServer(const char* IPAddr, int port);
  void RunServer();
};


#endif //SERVER_H_

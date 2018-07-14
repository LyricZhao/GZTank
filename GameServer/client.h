#ifndef CLIENT_H_
#define CLIENT_H_

#include "Tank.h"
#include <memory.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <assert.h>
#include <cstring>
#include <ctime>

class Client{
private:
  int m_clientSock;
  sockaddr_in m_servAddr;

  std::string m_userName;

  static void* Receive(void* clientSock);
public:
  Client();
  ~Client();
  Client(std::string userName);
  void ConnectToServer(const char* IPAddr, int port);
  void SendRequest();
};


#endif //CLIENT_H_

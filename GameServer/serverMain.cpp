#include <iostream>
#include "server.h"

int main() {
  Server server;
  server.SetServer("0.0.0.0", 8000);
  server.RunServer();
  return 0;
}

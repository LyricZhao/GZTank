#include <iostream>
#include "Tank.h"
#include "client.h"

int main() {
  Client client("lala0");
  client.ConnectToServer("167.99.97.59", 8001);
  client.SendRequest();
  return 0;
}

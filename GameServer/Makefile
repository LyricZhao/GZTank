CPPFLAGS = -std=c++11 -Wall -pthread -lpthread

All: client0 client1 server

client0: clientMain0.cpp client.cpp client.h Tank.cpp Tank.h
	g++ $(CPPFLAGS) clientMain0.cpp client.cpp Tank.cpp -o client0

client1: clientMain1.cpp client.cpp client.h Tank.cpp Tank.h
	g++ $(CPPFLAGS) clientMain1.cpp client.cpp Tank.cpp -o client1

server: serverMain.cpp server.cpp server.h Tank.cpp Tank.h
	g++ $(CPPFLAGS) serverMain.cpp server.cpp Tank.cpp -o server

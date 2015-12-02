#ifndef CLIENT_H_
#define CLIENT_H_
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>
#include <cstdlib>
#include <sstream>

using namespace std;

#define QUIT -1
#define SETUP 0
#define START 1
#define TURN 2
#define MOVE 3
#define LOST 4
#define UPDATE 5
#define CONFIMATION 6
#define DENIED 7

class Client {
private:
	string serverAddress;
	string serverPort;
	SOCKET connectSocket;

	bool gameInPlay = true;
	int playerState = 0;
	void sendMessage(string message);
	string receiveMessage();

public:
	Client(string ip = "127.0.0.1", string port = "8000") {
		serverAddress = ip;
		serverPort = port;
	}

	~Client() {
		// We're done, so close the socket connection and cleanup.
		closesocket(connectSocket);
		WSACleanup();
	}

	// Attempt to resolve the server IP address and port.
	// For now, we're going to connect to 127.0.0.1 (localhost)
	// For now, our server will open at port 8000.
	void connectToNetwork();
	void playGame();
};

#endif
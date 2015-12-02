// Battleship.Server.cpp : Defines the entry point for the console application.
//

// Batgseship.Server.cpp : Defines the entry point for the console application.
//
#pragma comment(lib, "Ws2_32.lib")
#include "GameState.h"
#include "ClientHandler.h"
#include <process.h>
#include <time.h> 

using namespace std;

DWORD WINAPI CreateHandler(LPVOID lpParameter);

int main()
{
	GameState gs = GameState();

	srand(time(NULL));

	cout << "Server Initialized" << endl;

	WSAData wsaData;

	// Initialize the WinSock library (v2.2)
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "Unable to initialize WinSock2: " << iResult;
	}

	// Set up a socket connection
	addrinfo *result = nullptr,
		*ptr = nullptr,
		hints;
	ZeroMemory(&hints, sizeof(hints));	// Clear the memory block in hints
	hints.ai_family = AF_INET;			// Our server will use IPv4 addressing
	hints.ai_socktype = SOCK_STREAM;	// We're using a socket stream...
	hints.ai_protocol = IPPROTO_TCP;	// ...and we're using TCP to establish a connection
	hints.ai_flags = AI_PASSIVE;		// We're going to be binding the returned IP address and port

										// Resolve the local address and port that the server will use
	string serverPort = "8000";			// For now, our server will listen at port 8000.
	iResult = getaddrinfo(nullptr, serverPort.c_str(), &hints, &result);
	if (iResult != 0) {
		cout << "Unable to resolve server address: " << iResult;
		WSACleanup();					// Connection failed, so clean up.
	}

	// Set up a socket to listen for messages by a client
	SOCKET listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		// Unable to set up a socket
		cout << "Unable to set up a socket: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
	}

	// Bind the socket so we can set up a TCP listening socket
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "Unable to bind a listening socket: " << WSAGetLastError();
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
	}

	// Free the address, we're done with it now.
	freeaddrinfo(result);

	// Grab a random number to determine
	// who goes first

	int player = rand() % 2;

	gs.setCurrentPlayer(player);

	gs.setGameRunning(true);

	while (gs.getNClients() != 2)
	{
		SOCKET clientSocket = INVALID_SOCKET;
		listen(listenSocket, SOMAXCONN);
		while (clientSocket == INVALID_SOCKET)
		{
			clientSocket = accept(listenSocket, NULL, NULL);
		}

		DWORD dwThreadId;
		gs.addSocket(clientSocket, gs.getNClients());
		gs.addClient();
		CreateThread(NULL, 0, CreateHandler, (LPVOID)&gs, 0, &dwThreadId);
	}


	while (gs.isGameRunning()) {}

	cout << "It is finished" << endl;

	cin.get();

	return 0;
}

DWORD WINAPI CreateHandler(LPVOID lpParameter)
{
	ClientHandler ch = ClientHandler((GameState *)lpParameter);
	ch.processClient();
	ch.~ClientHandler();
	return 0;
}



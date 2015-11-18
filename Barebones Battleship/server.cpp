#include <iostream>
#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <sstream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const int BUFFER_LENGTH = 512;

int main() {
	cout << "------" << endl;
	cout << "Server" << endl;
	cout << "------" << endl;

	WSAData wsaData;

	// Initialize the WinSock library (v2.2)
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "Unable to initialize WinSock2: " << iResult;
		return iResult;
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
		return 1;
	}

	// Set up a socket to listen for messages by a client
	SOCKET listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		// Unable to set up a socket
		cout << "Unable to set up a socket: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Bind the socket so we can set up a TCP listening socket
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "Unable to bind a listening socket: " << WSAGetLastError();
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// Free the address, we're done with it now.
	freeaddrinfo(result);
	
	SOCKET clientSocket[2];
	
	//The listen waits for two clients to join.
	int clientCount = 0;

	while (clientCount < 2){

		cout << "Awaiting players..." << endl;

		// Let's start listening.
		if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
			cout << "Unable to listen: " << WSAGetLastError() << endl;
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		// Now we'll start accepting a connection
		clientSocket[clientCount] = accept(listenSocket, NULL, NULL);
		if (clientSocket[clientCount] == INVALID_SOCKET) {
			cout << "Failed to accept a connection: " << WSAGetLastError();
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
	

	// Let's send the receiver a note telling them their player number.
		
	char receiverBuffer[BUFFER_LENGTH] = { 0 };
	int iSendResult;
	int receiverBufferLength = BUFFER_LENGTH;


	// Keep receiving until the peer shuts down its connection
	do {
		iResult = recv(clientSocket[clientCount], receiverBuffer, receiverBufferLength, 0);
		if (iResult > 0) {
			stringstream mess;
			mess << "You are player " << clientCount+1 << ".";

			//Convert the string to a char array.
			strncpy_s(receiverBuffer, mess.str().c_str(), sizeof(receiverBuffer));

			
			iSendResult = send(clientSocket[clientCount], receiverBuffer, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				cout << "Send failed: " << WSAGetLastError() << endl;
				closesocket(clientSocket[clientCount]);
				WSACleanup();
				return 1;
			}

			}
		else if (iResult == 0) {
			cout << "Client " << clientCount+1 << " has joined." << endl;
		}
		else {
			cout << "Failed to receive data: " << WSAGetLastError();
			closesocket(clientSocket[clientCount]);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);
	
	clientCount++;
	}

	//Now that two clients have joined, alert them to start the game.
	for (int x = 0; x < clientCount; x++){
		char newBuffer[BUFFER_LENGTH] = { 0 };
		string message = "The game is about to start!";
		//Convert the string to a char array.
		strncpy_s(newBuffer, message.c_str(), sizeof(newBuffer));
		send(clientSocket[x], newBuffer, BUFFER_LENGTH, 0);
	}

	//We're going to close those sockets here for now.
	for (int x = 0; x < clientCount; x++){
		// We're done sending and receiving, so close them.
		iResult = shutdown(clientSocket[x], SD_SEND);
		if (iResult == SOCKET_ERROR) {
			cout << "Shutdown failed: " << WSAGetLastError() << endl;
			closesocket(clientSocket[x]);
			WSACleanup();
			return 1;
		}
	}
	
	cout << "NOTE: Here's where the demo ends. The game would pick up right here in the final version." << endl;

	// Clean up
	for (int x = 0; x < clientCount; x++){
		closesocket(clientSocket[x]);
	}
	WSACleanup();

	return 0;
}
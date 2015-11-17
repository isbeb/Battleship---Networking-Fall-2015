#include <iostream>
#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>

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

	// Let's start listening.
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Unable to listen: " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// Now we'll start accepting a connection
	SOCKET clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Failed to accept a connection: " << WSAGetLastError();
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// Let's receive the data that the client send to us, and send a message back
	char receiverBuffer[BUFFER_LENGTH] = { 0 };
	int iSendResult;
	int receiverBufferLength = BUFFER_LENGTH;

	// Keep receiving until the peer shuts down its connection
	do {
		iResult = recv(clientSocket, receiverBuffer, receiverBufferLength, 0);
		if (iResult > 0) {
			cout << "Bytes received: " << iResult << endl;
			cout << "The server received: " << receiverBuffer << endl;

			// Send what the sender sent to us back to the sender
			iSendResult = send(clientSocket, receiverBuffer, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				cout << "Send failed: " << WSAGetLastError() << endl;
				closesocket(clientSocket);
				WSACleanup();
				return 1;
			}

			// Print out how many bytes we've sent
			cout << "Bytes sent: " << iSendResult << endl;
		}
		else if (iResult == 0) {
			cout << "The connection has been closed..." << endl;
		}
		else {
			cout << "Failed to receive data: " << WSAGetLastError();
			closesocket(clientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);

	// We're done sending and receiving, so close them.
	iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "Shutdown failed: " << WSAGetLastError() << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	// Clean up
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}
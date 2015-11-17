#include <iostream>
#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

const int BUFFER_LENGTH = 512;

int main() {
	cout << "------" << endl;
	cout << "Client" << endl;
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
	hints.ai_family = AF_UNSPEC;		// We don't care what IP address family we're using (IPv4 or IPv6)
	hints.ai_socktype = SOCK_STREAM;	// We're using a socket stream...
	hints.ai_protocol = IPPROTO_TCP;	// ...and uses TCP to establish a connection

	// Attempt to resolve the server IP address and port.
	string serverAddress = "127.0.0.1";	// For now, we're going to connect to 127.0.0.1 (localhost)
	string serverPort = "8000";			// For now, our server will open at port 8000.
	iResult = getaddrinfo(serverAddress.c_str(), serverPort.c_str(), &hints, &result);
	if (iResult != 0) {
		cout << "Unable to resolve server address: " << iResult;
		WSACleanup();					// Connection failed, so clean up.
		return 1;
	}

	// Let's create the socket
	ptr = result;		// We'll use the result that getaddrinfo returned to establish a connection
	SOCKET connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (connectSocket == INVALID_SOCKET) {
		// Connection failed
		cout << "Unable to create a socket connection: " << WSAGetLastError();
		freeaddrinfo(result);		// Clean up the address info we stored
		WSACleanup();				// Clean up the library we've opened
		return 1;
	}

	// Let's connect with the socket
	iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		// The socket connection couldn't be established, so close the socket and mark it as invalid
		closesocket(connectSocket);
		connectSocket = INVALID_SOCKET;
	}

	// TODO: Use the next address specified by getaddrinfo if the first one fails?

	// Now that we have established a connection (or not), free the address.
	freeaddrinfo(result);

	// If we couldn't connect to the server, we'll stop now.
	if (connectSocket == INVALID_SOCKET) {
		cout << "Unable to connect to the server" << endl;
		WSACleanup();
		return 1;
	}

	// Set up the sender and receiver buffers
	int receiverBufferLength = BUFFER_LENGTH;
	char *senderBuffer = "Hello from a client!";
	char receiverBuffer[BUFFER_LENGTH] = { 0 };

	// Send the initial buffer
	iResult = send(connectSocket, senderBuffer, (int)strlen(senderBuffer), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "We couldn't send a message to the server: " << WSAGetLastError() << endl;
		closesocket(connectSocket);	// Close the socket connection
		WSACleanup();				// Clean up the WinSock library
		return 1;
	}

	// Print out how much bytes were sent to the server
	cout << "Bytes sent: " << iResult << endl;

	// Now that we've finished sending data, let's close the sending connection. We can still
	// receive data via connectSocket though.
	iResult = shutdown(connectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "Unable to shut down the socket: " << WSAGetLastError() << endl;
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	// Wait until the server sends us a response
	do {
		iResult = recv(connectSocket, receiverBuffer, receiverBufferLength, 0);
		if (iResult > 0) {
			cout << "Bytes received: " << iResult << endl;
			cout << "The client received: " << receiverBuffer << endl;
		}
		else if (iResult == 0) {
			cout << "The connection was closed." << endl;
		}
		else {
			cout << "Unable to receive response: " << WSAGetLastError() << endl;
		}
	} while (iResult > 0);

	// We're done, so close the socket connection and cleanup.
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}
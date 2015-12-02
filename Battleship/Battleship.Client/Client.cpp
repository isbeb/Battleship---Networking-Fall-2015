#include "Client.h"


Client::Client(string ip, string port) {
	serverAddress = ip;
	serverPort = port;
	addrinfo *result = nullptr, *ptr = nullptr, hints;
	WSADATA wsaData;

	// Initialize the WinSock library (v2.2)
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "Unable to initialize WinSock2: " << iResult;
		system("exit");
	}

	ZeroMemory(&hints, sizeof(hints));	// Clear the memory block in hints
	hints.ai_family = AF_UNSPEC;		// We don't care what IP address family we're using (IPv4 or IPv6)
	hints.ai_socktype = SOCK_STREAM;	// We're using a socket stream...
	hints.ai_protocol = IPPROTO_TCP;	// ...and uses TCP to establish a connection

	int error = getaddrinfo(serverAddress.c_str(), serverPort.c_str(), &hints, &result);
	if (error != 0) {
		cout << "There was an error resolving the address: " << WSAGetLastError() << endl;
		WSACleanup();
		cin.get();
		system("exit");
	}


	// Let's create the socket
	ptr = result;		// We'll use the result that getaddrinfo returned to establish a connection
	connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (connectSocket == INVALID_SOCKET) {
		// Connection failed
		cout << "Unable to create a socket connection: " << WSAGetLastError();
		freeaddrinfo(result);		// Clean up the address info we stored
		WSACleanup();				// Clean up the library we've opened
		cin.get();
		system("exit");
	}

	// Let's connect with the socket
	iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cout << "There was an error connecting to the socket: " << WSAGetLastError() << endl;
		WSACleanup();
		cin.get();
		system("exit");
	}

	// TODO: Use the next address specified by getaddrinfo if the first one fails?

	// Now that we have established a connection (or not), free the address.
	freeaddrinfo(result);
}

void Client::sendMessage(string message)
{	
	int result = send(connectSocket, message.c_str(), (int)strlen(message.c_str()), 0);
	if (result == SOCKET_ERROR)
	{
		cout << "There was a problem sending the information: " << WSAGetLastError() << endl;
	}
}

string Client::receiveMessage()
{
	char recvbuf[512];

	ZeroMemory(recvbuf, sizeof(recvbuf));

	int result = recv(connectSocket, recvbuf, 512, 0);
	if (result == SOCKET_ERROR)
	{
		cout << "There was a problem receiving the information: " << WSAGetLastError() << endl;
	}

	return string(recvbuf);
}

bool Client::getSetupSignal()
{
	string newMessage = receiveMessage();

	if (newMessage == "SETUP")
	{
		return true;
	}
	else if (newMessage == "QUIT")
	{
		return false;
	}
}

void Client::confirm()
{
	sendMessage("CONFIRM");
}

void Client::deny()
{
	sendMessage("DENY");
}

void Client::sendMove(int x, int y)
{
	stringstream input;

	input << x << ',' << y;

	sendMessage(input.str());
}

void Client::exitGracefully()
{
	cout << "Opponent quit unexpectedly" << endl;
	cleanup();
	system("exit");
}

int Client::getUpdate()
{
	string newMessage = receiveMessage();
	
	if (newMessage == "HIT")
	{
		return 1;
	}
	else if (newMessage == "MISS")
	{
		return 0;
	}
	else if (newMessage == "WIN")
	{
		return -1;
	}
	else if (newMessage == "QUIT")
	{
		return -2;
	}
}

Move Client::getMove()
{
	string newMessage = receiveMessage();

	string x = newMessage.substr(0, newMessage.find(','));
	string y = newMessage.substr(newMessage.find(',') + 1, newMessage.length());

	Move opponentMove;

	opponentMove.x = atoi(x.c_str());
	opponentMove.y = atoi(y.c_str());

	return opponentMove;
}

void Client::sendUpdate(string result)
{
	sendMessage(result);
}

int Client::getTurn()
{
	string newMessage = receiveMessage();

	if (newMessage == "TURN")
	{
		return 1;
	}
	else if (newMessage == "NOTURN") // This gets attached to the message...?
	{
		return 0;
	}
	else if (newMessage == "QUIT")
	{
		return -1;
	}
}
#include "Client.h"
#include "Game.h"


void Client::connectToNetwork()
{
	addrinfo *result = nullptr, *ptr = nullptr, hints;
	WSADATA wsaData;

	// Initialize the WinSock library (v2.2)
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "Unable to initialize WinSock2: " << iResult;
	}

	ZeroMemory(&hints, sizeof(hints));	// Clear the memory block in hints
	hints.ai_family = AF_UNSPEC;		// We don't care what IP address family we're using (IPv4 or IPv6)
	hints.ai_socktype = SOCK_STREAM;	// We're using a socket stream...
	hints.ai_protocol = IPPROTO_TCP;	// ...and uses TCP to establish a connection

	int error = getaddrinfo(serverAddress.c_str(), serverPort.c_str(), &hints, &result);
	if (error != 0) {
		cout << "There was an error resolving the address: " << WSAGetLastError() << endl;
		WSACleanup();
	}


	// Let's create the socket
	ptr = result;		// We'll use the result that getaddrinfo returned to establish a connection
	connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (connectSocket == INVALID_SOCKET) {
		// Connection failed
		cout << "Unable to create a socket connection: " << WSAGetLastError();
		freeaddrinfo(result);		// Clean up the address info we stored
		WSACleanup();				// Clean up the library we've opened
	}

	// Let's connect with the socket
	iResult = SOCKET_ERROR;
	while (iResult == SOCKET_ERROR) {
		iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
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
		return "";
	}

	return string(recvbuf);
}

void Client::playGame() {
	string newMessage = "";
	int turns = 0;
	srand(time(NULL));
	// Wait until the server sends us a response
	while (gameInPlay) {
		switch (playerState) {
		case 0:
			newMessage = receiveMessage();

			if (newMessage == "SETUP") {
				playerState = 1;
				cout << "Connect! Time to setup the board!" << endl;
				setup();
				sendMessage("CONFIRM");
			}

			break;

		case 1:
			newMessage = receiveMessage();

			if (newMessage == "TURN")
			{
				cout << "Your turn player!" << endl;

				system("cls");
				DrawBoard(2, gameInPlay);

				//Get attack coords from this player
				bool goodInput = false;
				int x, y;
				while (goodInput == false)
				{
					goodInput = UserInputAttack(x, y, 1);
				}

				stringstream input;

				input << x << ',' << y;

				sendMessage(input.str());

				newMessage = receiveMessage();

				cout << "I made a move" << endl;

				if (newMessage == "HIT")
				{
					player[2].grid[x][y] = isHit;
				}
				else if (newMessage == "MISS")
				{
					player[2].grid[x][y] = isMiss;
				}
				else if (newMessage == "WIN")
				{
					cout << "Yay, I won!" << endl;
					gameInPlay = false;
				}
			}
			else if (newMessage == "NOTURN")
			{
				// Wait for update
				cout << "Other player's turn" << endl;
				newMessage = receiveMessage();

				string x = newMessage.substr(0, newMessage.find(','));
				string y = newMessage.substr(newMessage.find(',') + 1, newMessage.length());

				int ix = atoi(x.c_str());
				int iy = atoi(y.c_str());

				// Do something with the move here

				cout << "Opponent's move was " << ix << ", " << iy << endl;

				if (player[1].grid[ix][iy] == isShip)
				{
					if (GameOverCheck(1))
					{
						sendMessage("WIN");
						cout << "I lost :(" << endl;
					}
					else
					{
						sendMessage("HIT");
					}
				}
				else if (player[1].grid[ix][iy] == isWater)
				{
					sendMessage("MISS");
				}

				turns++;

			}
			break;
		case 2:
			break;
		}
	}
}
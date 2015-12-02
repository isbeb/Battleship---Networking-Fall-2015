#include "ClientHandler.h"

#define SETUP 0
#define CONFIRM 1
#define GETTURN 2
#define TURN 3
#define NOTURN 4

ClientHandler::ClientHandler(GameState * gs)
{
	state = gs;
	id = state->getNClients() - 1;
	mySocket = state->getSocket(id);
	playerState = 0;
}

ClientHandler::~ClientHandler()
{
	// We're done, so close the socket connection and cleanup.
	closesocket(mySocket);
	WSACleanup();
}

void ClientHandler::sendMessage(string message)
{
	int result = send(mySocket, message.c_str(), (int)strlen(message.c_str()), 0);
	if (result == SOCKET_ERROR)
	{
		cout << "There was a problem sending the information: " << WSAGetLastError() << endl;
	}
}

string ClientHandler::receiveMessage()
{
	char recvbuf[512];

	ZeroMemory(recvbuf, sizeof(recvbuf));

	int result = recv(mySocket, recvbuf, 512, 0);
	if (result == SOCKET_ERROR)
	{
		cout << "There was a problem receiving the information: " << WSAGetLastError() << endl;
	}
	
	return string(recvbuf);
}

void ClientHandler::processClient()
{
	int times = 0;

	string newMessage = "";

	while (state->isGameRunning())
	{
		switch (playerState) {
			// Now we wait for all clients and then let them know they can
			// can set up.
		case SETUP:
			sendMessage("SETUP");

			playerState = CONFIRM;
			break;

		case CONFIRM:
			newMessage = receiveMessage();

			if (newMessage == "CONFIRM") {
				state->addSetupClient();
				cout << "Client setup" << endl;

				while (!state->isAllSetup())
				{
					;
				}

				playerState = GETTURN;
			}

			break;
		case GETTURN:
			if (id == state->getCurrentPlayer()) {
				sendMessage("TURN");
				playerState = TURN;
			}
			else {
				sendMessage("NOTURN");
				playerState = NOTURN;
			}

			newMessage = receiveMessage();

			while (newMessage != "CONFIRM")
			{
				newMessage = receiveMessage();
			}

			break;
		case TURN:
			state->setBuffer(receiveMessage());
			state->setBufferSwitched(true);
			cout << "Got move" << endl;
			times = 0;
			while (state->hasBufferSwitched())
			{
				;
			}

			sendMessage(state->getBuffer());
			cout << "Sent current player's update" << endl;

			state->switchPlayer();
			state->setPlayerSwitched(true);
			playerState = GETTURN;
			break;

		case NOTURN:
			while (!state->hasBufferSwitched())
			{
				;
			}

			cout << "Got player update" << endl;

			sendMessage(state->getBuffer());
			cout << "Sent update to opponent" << endl;

			state->setBuffer(receiveMessage());

			state->setBufferSwitched(false);

			while (!state->hasPlayerSwitched())
			{
				;
			}

			state->setPlayerSwitched(false);

			playerState = GETTURN;
			break;
		}

		if (state->getBuffer() == "WIN")
		{
			newMessage = receiveMessage();

			if (newMessage == "CONFIRM")
			{
				playerState = 1;
			}
			else
			{
				state->setGameRunning(false);
			}
		}
	}
}
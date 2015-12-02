#include "ClientHandler.h"

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

	while (state->isGameRunning())
	{
		string newMessage = "";
		switch (playerState) {
			// Now we wait for all clients and then let them know they can
			// can set up.
		case 0:
			sendMessage("SETUP");

			playerState = 1;
			break;

		case 1:
			newMessage = receiveMessage();

			if (newMessage == "CONFIRM") {
				state->addSetupClient();
				cout << "Client setup" << endl;

				while (!state->isAllSetup())
				{
					;
				}

				playerState = 2;
			}

			break;
		case 2:
			if (id == state->getCurrentPlayer()) {
				sendMessage("TURN");
				playerState = 3;
			}
			else {
				sendMessage("NOTURN");
				playerState = 4;
			}
			break;
		case 3:
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
			playerState = 2;
			break;

		case 4:
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

			playerState = 2;
			break;
		}
	}
}
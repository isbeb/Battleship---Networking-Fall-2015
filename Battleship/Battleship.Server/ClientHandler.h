#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_
#include "GameState.h"
#include <iostream>

class ClientHandler {
private:
	int id;
	SOCKET mySocket;
	GameState * state;
	int playerState;

public:
	ClientHandler(GameState * gs);
	~ClientHandler();
	void sendMessage(string message);
	string receiveMessage();
	void processClient();

};


#endif
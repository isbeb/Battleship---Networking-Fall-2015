#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <WS2tcpip.h>
#include <Windows.h>
#include <string>

using namespace std;

class GameState {
private:
	SOCKET sockets[2];
	bool gameRunning = true;
	int nClients = 0;
	int setupClients = 0;
	string buffer;
	int currentPlayer = 1;
	bool bufferSwitched = false;
	bool playerSwitched = false;

public:
	GameState() {}
	~GameState() {}

	void addSocket(SOCKET socket, int box);
	SOCKET getSocket(int box);
	bool isAllAreConnected();
	bool isGameRunning();
	void setGameRunning(bool value);
	int getNClients();
	void addClient();
	void deleteClient();
	void addSetupClient();
	bool isAllSetup();
	string getBuffer();
	void setCurrentPlayer(int player);
	int getCurrentPlayer();
	void setBuffer(string message);
	void setBufferSwitched(bool value);
	bool hasBufferSwitched();
	void switchPlayer();
	bool hasPlayerSwitched();
	void setPlayerSwitched(bool value);
};

#endif
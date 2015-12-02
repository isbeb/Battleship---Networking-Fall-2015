#include "GameState.h"

void GameState::addSocket(SOCKET socket, int box)
{
	sockets[box] = socket;
}

SOCKET GameState::getSocket(int box)
{
	return sockets[box];
}

bool GameState::isAllAreConnected()
{
	if (nClients == 2) {
		return true;
	}

	return false;
}

bool GameState::isGameRunning()
{
	return gameRunning;
}

void GameState::setGameRunning(bool value)
{
	gameRunning = value;
}

int GameState::getNClients() {
	return nClients;
}

void GameState::addClient() {
	nClients++;
}

void GameState::deleteClient() {
	nClients--;
}

void GameState::addSetupClient() {
	setupClients++;
}

bool GameState::isAllSetup() {
	if (setupClients == 2) {
		return true;
	}

	return false;
}

string GameState::getBuffer() {
	return buffer;
}

void GameState::setCurrentPlayer(int player) {
	currentPlayer = player;
}

int GameState::getCurrentPlayer() {
	return currentPlayer;
}

void GameState::setBuffer(string message)
{
	buffer = message;
}

void GameState::setBufferSwitched(bool value)
{
	bufferSwitched = value;
}

bool GameState::hasBufferSwitched()
{
	return bufferSwitched;
}

void GameState::switchPlayer()
{
	if (currentPlayer == 0)
	{
		currentPlayer = 1;
	}
	else
	{
		currentPlayer = 0;
	}
}

bool GameState::hasPlayerSwitched()
{
	return playerSwitched;
}

void GameState::setPlayerSwitched(bool value)
{
	playerSwitched = value;
}
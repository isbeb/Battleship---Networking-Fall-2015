#include "Client.h"

int main()
{
	Client c = Client();
	c.connectToNetwork();
	c.playGame();
}
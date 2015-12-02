#ifndef GAME_H_
#define GAME_H_
#include <string>
using namespace std;

const int boardWidth = 15;
const int boardHeight = 10;
const int numberOfShipTypes = 5;

const char isWater = 247; //ASCII Character Code
const char isHit = 'X';
const char isShip = 'S';
const char isMiss = '0';

struct LocationOnGrid
{
	//A location on the grid defined
	//by X(horizontal) Y(vertical) coordinates
	int X;
	int Y;
};

struct Ship
{
	std::string name;
	int length;
	LocationOnGrid onGrid[5];   //0-4 max length of biggest ship
	bool hitFlag[5];            //Whether or not those points are a "hit"
} ship[numberOfShipTypes];

struct Player
{
	char grid[boardWidth][boardHeight];
} player[3]; //Ignore player 0, just using player's 1 & 2

enum Direction { horizontal, vertical };

struct PlaceShips
{
	Direction direction;
	Ship shipType;
};

//Functions
void LoadShips();
void ResetBoard();
void DrawBoard(int, bool);
PlaceShips UserInputShipPlacement();
bool UserInputAttack(int&, int&, int);
bool GameOverCheck(int);
void setup();


#endif
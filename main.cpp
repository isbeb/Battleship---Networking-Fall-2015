#include <iostream>
#include <string>
#include <stdlib.h>

/*
Notes:

*/

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
    string name;
    int length;
    LocationOnGrid onGrid[5];   //0-4 max length of biggest ship
    bool hitFlag[5];            //Whether or not those points are a "hit"
} ship[numberOfShipTypes];

struct Player
{
    char grid[boardWidth][boardHeight];
} player[3]; //Ignore player 0, just using player's 1 & 2

enum Direction {horizontal,vertical};

struct PlaceShips
{
    Direction direction;
    Ship shipType;
};

bool gameRunning = false;

//Functions
void LoadShips();
void ResetBoard();
void DrawBoard(int);
PlaceShips UserInputShipPlacement();
bool UserInputAttack(int&,int&,int);
bool GameOverCheck(int);

int main()
{
    LoadShips();
    ResetBoard();

    //"PLACE SHIPS" phase of game
    //Loop through each player...
    for (int aPlayer=1; aPlayer<3; ++aPlayer)
    {
        //Loop through each ship type to place
        for (int thisShip=0; thisShip<numberOfShipTypes; ++thisShip)
        {
            //Display game board for player
            system("cls");
            DrawBoard(aPlayer);
            //Give instructions
            cout << "\n";
            cout << "INSTRUCTIONS (Player " << aPlayer << ")";
            cout << "\n";
            cout << "You are about to place your ships.";
            cout << "\n";
            cout << "Format should be:";
            cout << "\n";
            cout << "Facing (0:Horizontal, 1:Vertical), X (top-row), Y (left-side)";
            cout << "\n";
            cout << "Example: 0 7 2    Places a ship beginning at X:7 Y:2 going horizontal.";
            cout << "\n";
            cout << "\n";
            cout << "Ship to place: " << ship[thisShip].name << " which has a length of " << ship[thisShip].length;
            cout << "\n";
            cout << "Where do you want it placed?";

            //Get input from user and loop until good data is returned
            PlaceShips aShip;
            aShip.shipType.onGrid[0].X = -1;
            while (aShip.shipType.onGrid[0].X == -1)
            {
                aShip = UserInputShipPlacement();
            }

            //Combine user data with "this ship" data
            aShip.shipType.length = ship[thisShip].length;
            aShip.shipType.name = ship[thisShip].name;

            //Add the FIRST grid point to the current player's game board
            player[aPlayer].grid[aShip.shipType.onGrid[0].X][aShip.shipType.onGrid[0].Y] = isShip;

            //Determine ALL grid points based on length and direction
            for (int i=1; i<aShip.shipType.length; ++i)
            {
                if (aShip.direction == horizontal)
                {
                    aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X+1;
                    aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y;
                }
                if (aShip.direction == vertical)
                {
                    aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y+1;
                    aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X;
                }

                //Add the REMAINING grid points to our current players game board
                player[aPlayer].grid[aShip.shipType.onGrid[i].X][aShip.shipType.onGrid[i].Y] = isShip;
            }
            //Loop back through each ship type
        }
        //Loop back through each player
    }

    //********* FINISHED WITH "PLACE SHIPS" PHASE *********************************
    //*****************************************************************************

    //Ready to play the game
    gameRunning = true;
    int thisPlayer = 1;
    do
    {
        //Because we are ATTACKING now, the
        //opposite players board is the display board
        int enemyPlayer;
        if (thisPlayer == 1) enemyPlayer = 2;
        if (thisPlayer == 2) enemyPlayer = 1;
        system("cls");
        DrawBoard(enemyPlayer);

        //Get attack coords from this player
        bool goodInput = false;
        int x,y;
        while (goodInput == false)
        {
            goodInput = UserInputAttack(x,y,thisPlayer);
        }

        //Check board; if a ship is there, set as HIT.. otherwise MISS
        if (player[enemyPlayer].grid[x][y] == isShip) player[enemyPlayer].grid[x][y] = isHit;
        if (player[enemyPlayer].grid[x][y] == isWater) player[enemyPlayer].grid[x][y] = isMiss;

        //Check to see if the game is over
        //If 0 is returned, nobody has won yet
        int aWin = GameOverCheck(enemyPlayer);
        if (aWin != 0)
        {
            gameRunning = false;
            break;
        }
        //Alternate between each player as we loop back around
        thisPlayer = (thisPlayer == 1) ? 2 : 1;
    }
    while (gameRunning);

    system("cls");
    cout << "\n";
    cout << "\n";
    cout << "CONGRATULATIONS!  PLAYER: " << thisPlayer << " HAS WON THE GAME!";
    cout << "\n";
    cout << "\n";
    cout << "\n";
    cout << "\n";

    system("pause");
    return 0;
}


bool GameOverCheck(int enemyPLAYER)
{
    bool winner = true;
    //Loop through enemy board
    for (int w=0; w<boardWidth; ++w)
    {
        for (int h=0; h<boardHeight; ++h)
        {
            //If any ships remain, game is NOT over
            if (player[enemyPLAYER].grid[w][h] = isShip)
            {
                winner = false;
                return winner;
            }
        }
    }
    //If we get here, somebody won, game over!
    return winner;
}


bool UserInputAttack(int& x, int& y, int theplayer)
{
    cout << "\nPLAYER " << theplayer << ", ENTER COORDINATES TO ATTACK: ";
    bool goodInput = false;
    cin >> x >> y;
    if (x<0 || x>=boardWidth) return goodInput;
    if (y<0 || y>=boardHeight) return goodInput;
    goodInput = true;
    return goodInput;
}

PlaceShips UserInputShipPlacement()
{
    int d, x, y;
    PlaceShips tmp;
    //Using this as a bad return
    tmp.shipType.onGrid[0].X = -1;
    //Get 3 integers from user
    cin >> d >> x >> y;
    if (d!=0 && d!=1) return tmp;
    if (x<0 || x>=boardWidth) return tmp;
    if (y<0 || y>=boardHeight) return tmp;
    //Good data
    tmp.direction = (Direction)d;
    tmp.shipType.onGrid[0].X = x;
    tmp.shipType.onGrid[0].Y = y;
    return tmp;
}

void LoadShips()
{
    //Sets the default data for the ships
    //we plan to include in the game
    //IMPORTANT!! > MUST MATCH SHIP_TYPES -Default=5 (0-4)
    ship[0].name = "Cruiser";
    ship[0].length = 2;
    ship[1].name = "Frigate";
    ship[1].length = 3;
    ship[2].name = "Submarine";
    ship[2].length = 3;
    ship[3].name = "Escort";
    ship[3].length = 4;
    ship[4].name = "Battleship";
    ship[4].length = 5;
}
void ResetBoard()
{
    //Loop through each player
    for (int plyr=1; plyr<3; ++plyr)
    {
        //For each grid point, set contents to 'water'
        for (int w=0; w<boardWidth; ++w)
        {
            for (int h=0; h<boardHeight; ++h)
            {
                player[plyr].grid[w][h] = isWater;
            }
        }
        //Loop back to next player
    }
}

void DrawBoard(int thisPlayer)
{
    //Draws the board for a player (thisPlayer)
    cout << "PLAYER " << thisPlayer << "'s GAME BOARD\n";
    cout << "----------------------\n";

    //Loop through top row (board_width) and number columns
    cout << "   ";
    for (int w=0; w<boardWidth; ++w)
    {
        if (w < 10)
            //Numbers only 1 character long, add two spaces after
            cout << w << "  ";
        else if (w >= 10)
            //Numbers 2 characters long, add only 1 space after
            cout << w << " ";
    }
    cout << "\n";

    //Loop through each grid point and display to console
    for (int h=0; h<boardHeight; ++h)
    {
        for (int w=0; w<boardWidth; ++w)
        {

            //If this is the FIRST (left) grid point, number the grid first
            if (w==0) cout << h << " ";
            //If h was 1 character long, add an extra space to keep numbers lined up
            if (w<10 && w==0) cout << " ";
            //Display contents of this grid (if game isn't running yet, we are placing ships
            //so display the ships
            if (gameRunning == false) cout << player[thisPlayer].grid[w][h] << "  ";
            //Don't show ships, BUT show damage if it's hit
            if (gameRunning == true && player[thisPlayer].grid[w][h] != isShip)
            {
                cout << player[thisPlayer].grid[w][h] << "  ";
            }
            else if (gameRunning == true && player[thisPlayer].grid[w][h] == isShip)
            {
                cout << isWater << "  ";
            }
            //If we have reached the border.. line feed
            if (w == boardWidth-1) cout << "\n";
        }
    }
}

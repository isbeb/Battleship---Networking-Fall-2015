/*
Intial 
	Author: Harrison Kren
	Date Created: 11/19/15
	Purpose: console for game battleship

Modifications
	Author: 
	Date:
	Modifed: 
*/

#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include "board.h"

//############################ Methods list for console methods ###########################

using namespace std; 

bool UserInputShip(int ship_number, int& x, int& y, int& d, Board gameboard)
{
	// causes infinate loop on char inputs
    bool goodInput = false;
	cin >> d >> x >> y;

	if(cin.fail())
	{
		cin.clear();
		cin.ignore(); 
		cout << "\nPlease enter an Integer only." << endl;
	}

	for (int i=0; i<gameboard.get_ship_size(ship_number); i++)
	{
		if(d == 0)
		{
			if(gameboard.get_ship(x+i,y)) return goodInput;
			if (x<0 || x>=10-gameboard.get_ship_size(ship_number)-i) return goodInput;
			if (y<0 || y>=10) return goodInput;
		}

		else if(d == 1)
		{
			if(gameboard.get_ship(x,y+i)) return goodInput;
			if (x<0 || x>=10) return goodInput;
			if (y<0 || y>=10-gameboard.get_ship_size(ship_number)-i) return goodInput;
		}

		else
			return goodInput;
	}
    goodInput = true;
    return goodInput;
}

void auto_set(Board& gameboard)
{
	for (int i=0; i<5; i++)
	{
		int d,x,y;
		d = (rand()%2);
		bool goodInput;

		do{
			goodInput = true;
			if (d == 0)
			{ 
				x = (rand()%(10-gameboard.get_ship_size(i)));
				y = (rand()%10);
				for (int j=0; j<gameboard.get_ship_size(i); j++)
				{
					if(gameboard.get_ship(x+j,y))
					{
					goodInput = false;
					break;
					}
				}
			}// end horizontal

			if (d == 1)
			{
				x = (rand()%10);
				y = (rand()%(10-gameboard.get_ship_size(i)));
				for (int j=0; j<gameboard.get_ship_size(i); j++)
				{
					if(gameboard.get_ship(x,y+j))
					{
					goodInput = false;
					break;
					}
				}
			}// end vertical

		}while(goodInput == false);

		gameboard.place_ship(x, y, d, i);
	}
};

bool auto_shoot(int& x, int& y, Board playerboard)
{
	bool goodInput = false;
    x = (rand()%10);
	y = (rand()%10);
    if(playerboard.get_shot(x,y)) return goodInput;
    goodInput = true;
    return goodInput;
};

void set_phase(Board& gameboard)
{
        
		system("cls");
        std:: cout << gameboard.display_gameboard();
		cout << "\n";
        cout << "Would you like to auto set your ships? 0 for no and 1 for yes";
        cout << "\n";
		int a;
		bool validInput = false;
		while (validInput == false)
		{
			cin >> a;
			if(cin.fail())
			{
				cin.clear();
				cin.ignore(); 
				cout << "\nPlease enter an Integer only." << endl;
			}

			if (a == 0){
			
				//Loop through each ship type to place
				for (int thisShip=0; thisShip<5; ++thisShip)
				{

					//Display game board for player
					system("cls");
					std:: cout << gameboard.display_gameboard();
					//Give instructions
					cout << "\n";
					cout << "INSTRUCTIONS (Player)";
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
					cout << "Ship to place: " << gameboard.get_ship_name(thisShip);
					cout << " which has a length of " << gameboard.get_ship_size(thisShip);
					cout << "\n";
					cout << "Where do you want it placed?";

					//Get input
					int d, x, y;
					bool goodInput = false;
					while (goodInput == false)
					{
						goodInput = UserInputShip(thisShip, x, y, d, gameboard);
						if (goodInput == false) cout << "\n Invalid input try again.";
					}
					gameboard.place_ship(x, y, d, thisShip);
				}
			validInput = true;
			}

		else if (a == 1)
		{
			auto_set(gameboard);
			validInput = true;
		}
		else
		{
			cout << "\n";
			cout << "Invalid input try again.";
		}
	}
};

bool UserInputAttack(int& x, int& y, Board playerboard)
{
    cout << "\nPLAYER, ENTER COORDINATES TO ATTACK: ";
    bool goodInput = false;
    cin >> x >> y;
	if(cin.fail())
		{
			cin.clear();
			cin.ignore(); 
			cout << "\nPlease enter an Integer only." << endl;
			return goodInput;
		}
    if (x<0 || x>=10) return goodInput;
    if (y<0 || y>=10) return goodInput;
	if(playerboard.get_shot(x,y)) return goodInput;
    goodInput = true;
    return goodInput;
}

void check_shot(int x, int y, Board& player, bool shot) 
{
	if (shot == true) // hit
		{
			player.fired_shot(x,y,true); // hit
		}
	if (shot == false)
		{
			player.fired_shot(x,y,false); // miss
		}
};

void shot_display(int x, int y, Board player, bool shot, string name)
{
	system("cls");
	cout << player.display_gameboard();
	if(shot == true) cout << name <<  " shot: " << x << " " << y << " Was a hit\n";
	if(shot == false) cout << name <<  " shot: " << x << " " << y << " Was a miss\n";
	system("pause");
};

bool battle_phase(Board player, Board opponent)
{
	bool gameRunning = true;
    do
    {
		//display gameboard
		system("cls");
        cout << player.display_gameboard();

        //Get attack coords from this player
		int x,y;
        bool goodInput = false;
		//while (goodInput == false)
		//{
        //   goodInput = UserInputAttack(x,y,player);
		//}

        //Check if it is a hit
		auto_shoot(x, y, player);
		bool shot = opponent.received_shot(x,y); // Info sent from server later
		check_shot( x, y, player, shot);

		//Display of your shot 
		shot_display(x, y, player, shot, "Player");

		//oponenets turn
		auto_shoot(x, y, opponent);
		shot = player.received_shot(x,y); // Info sent from server later
		check_shot( x, y, opponent, shot);

		//Display of your shot after opponet fires
		shot_display( x, y, player, shot, "Opponent");

        //Check to see if the game is over
        //If 2 is returned no one has won yet, 0 you won, 1 you lost
		int aWin = player.victory_test();
        if (aWin != 2)
        {
			if (aWin == 0) cout << "\nVictory!\n";
			else if (aWin == 1) cout << "\nDeafeat!\n";
            gameRunning = false;
            break;
        };
    }
    while (gameRunning);

	cout << "Play again?(0 for no and 1 for yes)\n";
	 int choice;
     cin >> choice;
	 if( choice == 1)
		 return false;
	 else
		 return true;
};

//################################## Main method #######################################
int main(){
	bool play_again = false;
	do
	{
		Board playerboard;
		Board opponentboard;

		system("cls");

		auto_set(opponentboard);

		set_phase(playerboard);

		play_again = battle_phase(playerboard, opponentboard);

	} while( play_again == false);

return 1;
} // end of main
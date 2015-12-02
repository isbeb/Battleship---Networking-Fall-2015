/*
Intial 
	Author: Harrison Kren
	Date Created: 11/19/15
	Purpose: console for game battleship

Last Modified
	Author: Dillon Broadus 
	Date: 11/30/15
	Purpose: added networking components
*/

#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include "board.h"
#include "Client.h"

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

void auto_shoot(int& x, int& y, Board playerboard)
{
	bool goodInput = false;

	while (goodInput == false)
	{
		x = (rand()%10);
		y = (rand()%10);
		if(playerboard.get_shot(x,y) == false) goodInput = true;
	}
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

void mark_shot(int x, int y, Board& player, bool shot) 
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

bool battle_phase(Board player, Client c)
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

		int turn = c.getTurn(); // Get turn
		c.confirm(); // Tell the server it's acknowledged
		if (turn == 1) // This client's turn
		{ 
			// Get player input
			while (goodInput == false)
			{
				goodInput = UserInputAttack(x, y, player);
			}

			//Good for testing
			//uncomment for autofiring 
			//auto_shoot(x, y, player);

			// Send move out
			c.sendMove(x, y);

			int shot = c.getUpdate(); // Info sent from server saying whether the players shot was a hit(true) or miss(false)

			if (shot == 1) // Hit
			{
				shot_display(x, y, player, false, "\nPlayer");  //Display of your shot
				mark_shot(x, y, player, true); //marks shot onto players board
			}
			else if (shot == 0) // Miss
			{
				shot_display(x, y, player, false, "\nPlayer");  //Display of your shot
				mark_shot(x, y, player, false); //marks shot onto players board
			}
			else if (shot == -1) // Win
			{
				cout << "\nVictory!\n";
				gameRunning = false;
			}
			else if (shot == -2) // Quit
			{
				c.exitGracefully();
			}	 
		}
		else if (turn == 0) // Not this player's turn
		{
			Move o = c.getMove();

			bool oShot = player.received_shot(o.x, o.y); // where player would check if the shot was a hit or miss

			shot_display(o.x, o.y, player, oShot, "\nOpponent"); //Display of your shot after opponet fires
			
			int aWin = player.victory_test();

			if (aWin == 1)
			{
				cout << "\nDefeat!\n";
				c.sendUpdate("WIN"); // Let the other player know they've won
				
				gameRunning = false;
			}
			else
			{
				if (oShot == true)
				{
					c.sendUpdate("HIT"); // Let the other player know of a hit
				}
				else
				{
					c.sendUpdate("MISS"); // Let the other player know of a miss
				}
			}

		}
		else // Something not good happened
		{
			cout << "Happened in the getTurn branch" << endl;
			c.exitGracefully();
		}
    }
    while (gameRunning);

	cout << "Play again?(0 for no and 1 for yes)\n";
	int choice;
    cin >> choice;
	if (choice == 1)
	{
		c.confirm(); // yes
		return true;
	}
	else {
		c.deny(); // no
		return false;
	}
};

//################################## Main method #######################################
int main(){

	Client c = Client();
	cout << "Client successfully connected" << endl;

	cout << "Waiting for other player..." << endl;

	if (c.getSetupSignal())
	{
		bool play_again = true;
		do
		{
			Board playerboard;
			Board opponent;

			system("cls");

			set_phase(playerboard);

			c.confirm();

			play_again = battle_phase(playerboard, c);

		} while (play_again == true);

		c.~Client();
    }
	else
	{
		c.exitGracefully();
	}

return 1;
} // end of main
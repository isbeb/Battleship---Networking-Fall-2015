/*
Intial 
	Author: Harrison Kren
	Date Created: 11/19/15
	Purpose: Board class for game battleship

Modifications
	Author: 
	Date:
	Modifed: 
*/

#include "board.h"

std::string Board:: display_gameboard()
{
	std::string output = "         player board                     opponent board \n  0  1  2  3  4  5  6  7  8  9      0  1  2  3  4  5  6  7  8  9 \n";
	for (int h=0; h<10; ++h)
    {
		output = output + std::to_string(h);

        for (int w=0; w<20; ++w)
        {
			output = output + " " + gameboard[h][w].display() + " ";

			if(w == 9)
				output = output + "   " + std::to_string(h);
		} // end width
		output = output + "\n";
	} // end hight

	return output;

};

bool Board:: get_ship(int x,int y)
{
	return gameboard[y][x].get_ship();
};

std::string Board:: get_ship_name(int ship_number)
{
	return ship_array[ship_number].get_name();
};

int Board:: get_ship_size(int ship_number)
{
	return ship_array[ship_number].get_size();
};

void Board:: place_ship(int x, int y, int h_v, int ship_number)
{
	if (h_v == 0)
	{
		for (int h=0; h < ship_array[ship_number].get_size(); h++)
		{
			gameboard[y][x+h].set_ship();
			//set tile so it holds ship placed name to do
		}
	}
	else if (h_v == 1)
	{
		for (int v=0; v < ship_array[ship_number].get_size(); v++)
		{
			gameboard[y+v][x].set_ship();
			//set tile so it holds ship placed name
		}
	}
};

int Board:: victory_test(){
	if(win_counter == max_hits)
		return 0;
	else if (lose_counter == max_hits)
		return 1;
	else
		return 2;
};

bool Board:: received_shot(int x, int y)
{
	if(gameboard[y][x].get_water() == true)
	{ 
		gameboard[y][x].set_shot();
		return false;
	}
	if (gameboard[y][x].get_ship() == true)
	{
		gameboard[y][x].set_shot();
		lose_counter++;
		// add increment to counter
		return true;
	}

}; // returns flase for miss and true for hit also sets tiles

void Board:: fired_shot(int x, int y, bool shot)
{
	if(shot == true)
	{
		gameboard[y][x+10].set_ship();
		gameboard[y][x+10].set_shot();
		win_counter++;
	}
	if(shot == false)
		gameboard[y][x+10].set_shot();
};

bool Board:: get_shot(int x,int y)
{
	return gameboard[y][x+10].get_shot();
};

int Board:: get_win_counter()
{
	return win_counter;
};

int Board:: get_lose_counter()
{
	return lose_counter;
};

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

#ifndef BOARD
#define BOARD
#include <iostream>
#include <string>
#include "tile.h"
#include "ship.h"

class Board
{
	private: 
		int max_hits;
		int win_counter;
		int lose_counter;
		Tile gameboard [10][20];
		Ship ship_array[5];

	public: 
		// deafult constructor
		Board(): max_hits(17), win_counter(0), lose_counter(0)
		{
			// fill board array with deafault tiles
			for (int h=0; h<10; h++)
			{
				for (int w=0; w<20; w++)
				{
					gameboard [h][w] = Tile();
				} // w for loop
			} // h for loop
			
			// fill ship array
			ship_array[0].set_name("Cruiser");
			ship_array[0].set_size(2);
			ship_array[1].set_name("Frigate");
			ship_array[1].set_size(3);
			ship_array[2].set_name("Submarine");
			ship_array[2].set_size(3);
			ship_array[3].set_name("Escort");
			ship_array[3].set_size(4);
			ship_array[4].set_name("Battleship");
			ship_array[4].set_size(5);
		}
		
			void place_ship(int x, int y, int h_v, int ship_number);
			bool get_ship(int x, int y);
			std::string get_ship_name(int ship_number);
			void set_ship_name(int x,int y, int ship_number);
			int get_ship_size(int ship_number);
			int victory_test();
			std::string display_gameboard();
			bool received_shot(int x, int y);
			void fired_shot(int x, int y, bool shot);
			bool get_shot(int x,int y);
			int get_win_counter();
			int get_lose_counter();
		
};
#endif
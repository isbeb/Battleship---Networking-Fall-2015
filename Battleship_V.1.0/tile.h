/*
Intial 
	Author: Harrison Kren
	Date Created: 11/19/15
	Purpose: tile class for game battleship

Modifications
	Author: 
	Date:
	Modifed: 
*/

#ifndef TILE
#define TILE
#include <iostream>
#include <string>

class Tile
{
	private: 
		bool water_flag, ship_flag, shot_flag;
		std::string ship_name;

	public: 
		// deafult constructor
		Tile(): water_flag(true), ship_flag(false), shot_flag(false), ship_name("n/a") {}
		std::string display();
		bool get_water();
		void set_water();
		bool get_ship();
		void set_ship();
		bool get_shot();
		void set_shot();
		std::string get_ship_name();
		void set_ship_name(std::string my_ship_name);

};
#endif
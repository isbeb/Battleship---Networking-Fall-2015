/*
Intial 
	Author: Harrison Kren
	Date Created: 11/19/15
	Purpose: Ship class for game battleship

Modifications
	Author: 
	Date:
	Modifed: 
*/

#ifndef SHIP
#define SHIP
#include <iostream>
#include <string>

class Ship
{
	private: 
		std::string ship_name;
		int counter;
		int size;
		bool sunk_flag;

	public: 
		// deafult constructor
		Ship(): ship_name("n/a"), counter(0), size(0), sunk_flag(false) {}

		void set_name(std::string my_ship_name);
		std::string get_name();
		void set_size(int my_size);
		int get_size();
		int get_counter();
		void increment_counter();
		void set_sunk(bool my_sunk_flag);
		bool get_sunk();
};
#endif


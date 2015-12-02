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

#include "ship.h"

void Ship:: set_name(std::string my_ship_name)
{
	ship_name = my_ship_name;
};

std::string Ship:: get_name()
{
	return ship_name;
};

void Ship:: set_size(int my_size)
{
	size = my_size;
};
		
int Ship:: get_size()
{
	return size;
};

int Ship:: get_counter()
{
	return counter;
};
		
void Ship:: increment_counter()
{
	counter = counter++;
};

void Ship:: set_sunk(bool my_sunk_flag)
{
	sunk_flag = my_sunk_flag;
};

bool Ship:: get_sunk()
{
	return sunk_flag;
};
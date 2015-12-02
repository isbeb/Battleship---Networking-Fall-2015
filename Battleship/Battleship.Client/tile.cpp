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

#include "tile.h"

bool Tile:: get_water()
{
	return water_flag;
};
	
void Tile:: set_water()
{
	water_flag = true;
	ship_flag =false;
};
	
bool Tile:: get_ship()
{
	return ship_flag;
};
	
void Tile:: set_ship()
{
	ship_flag = true;
	water_flag =false;
};
	
bool Tile:: get_shot()
{
	return shot_flag;	
};
	
void Tile:: set_shot()
{
	shot_flag = true;
};
	
std:: string Tile:: get_ship_name()
{
	return ship_name;
};
	
void Tile:: set_ship_name(std::string my_ship_name)
{
	ship_name = my_ship_name;
};

std::string Tile:: display()
{
	std::string string = string;
	if ( water_flag == true && shot_flag == false)
		return string = 247; //ASCII Character Code
	else if ( water_flag == true && shot_flag == true)
		return string = 'O';
	else if (ship_flag == true && shot_flag == false)
		return string = 'S';
	else if (ship_flag == true && shot_flag == true)
		return string ='X';
	else
		return string = 'f';
};
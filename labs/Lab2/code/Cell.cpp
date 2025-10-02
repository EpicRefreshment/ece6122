/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the source file for the ECE_Defender class and implements all functions
and maintains all variables defined in ECE_Defender.h.
This class is derived from the SFML Window class.
ECE_Defender manages all game objects, state, and logic. 

*/

#include "Cell.h"

using namespace sf;
using namespace std;

/*
This is the constructor for ECE_Defender. This function initializes the window
and initializes all game logic variables. Calls other helper functions to initialize
different aspects of the game.

Arguments:
    N/A

Return Values:
    ECE_Defender
*/
Cell::Cell(float size, int row, int column)
{
    this->setSize({size, size});
    this->setFillColor(Color::White);

    this->row = row;
    this->column = column;
    this->setPosition(row * size, column * size);

    alive = false;
}

void Cell::setDead()
{
    alive = false;
}

void Cell::setAlive()
{
    alive = true;
}

bool Cell::isAlive()
{
    return alive;
}
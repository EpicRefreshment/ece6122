/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the header file for the ECE_Buzzy class.
This class is derived from the SFML Sprite class.
ECE_Buzzy manages details for Buzzy, initiatilizes the object,
maintains position, manages spawn location, moves Buzzy, and detects collisions with other objects
and fires laser.

*/

#pragma once

// Include SFML libraries here
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Cell: public RectangleShape
{
public:
    /***************************
      Public member functions
    ****************************/

    // Constructor
    Cell(float size, int x, int y);

    void setDead();
    void setAlive();

    bool isAlive();

private:
    /***************************
      Private member functions
    ****************************/
    bool alive;

    int row;
    int column;

    /***************************
      Private member variables
    ****************************/
};
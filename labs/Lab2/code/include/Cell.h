/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 10/08/2025

Description:

Lab 2

This is the header file for the Cell class.
This class is derived from the SFML RectangleShape class.
This class primarily exists to provide an object with the necessary
information to draw the cell on the grid.

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
    Cell(float size, int row, int column, int offsetX, int offsetY);

private:
    /***************************
      Private member functions
    ****************************/

    /***************************
      Private member variables
    ****************************/
};
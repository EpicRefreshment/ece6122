/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 10/08/2025

Description:

Lab 2

This is the source file for the Cell class and implements all functions
and maintains all variables defined in Cell.h.
This class is derived from the SFML RectangleShape class.
This class primarily exists to provide an object with the necessary
information to draw the cell on the grid.

*/

#include "Cell.h"

using namespace sf;
using namespace std;

/*
This is the constructor for Cell.
It sets the size, color, and position of the rectangle.
Calculates position based on given row and column.

Arguments:
    size - size of rectangle
    row - given row of cell, multiplied by size to get X display position
    column - given column of cell, multiplied by size to get Y display position
    offsetX - provides offset to center cell grid on x-axis in case there is empty space in display
    offsetY - provides offset to center cell grid on y-axis in case there is empty space in display

Return Values:
    Cell
*/
Cell::Cell(float size, int row, int column, int offsetX, int offsetY)
{
    this->setSize({size, size});
    this->setFillColor(Color::White);

    float positionX = (column * size) + (offsetX / 2); // center grid on x axis using offset
    float positionY = (row * size) + (offsetY / 2); // center grid on y axis using offset
    this->setPosition(positionX, positionY);
}
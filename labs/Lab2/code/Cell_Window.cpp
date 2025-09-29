/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the source file for the ECE_Defender class and implements all functions
and maintains all variables defined in ECE_Defender.h.
This class is derived from the SFML Window class.
ECE_Defender manages all game objects, state, and logic. 

*/

#include "Cell_Window.h"

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
Cell_Window::Cell_Window()
{
    vm = VideoMode(800, 600); // Set window size
    this->create(vm, "Cellular Automata", Style::Default); // Create the window

    windowSize = this->getSize(); // Get the actual window size

    numThreads = 1;
    cellSize = 5.0f;
    gridWidth = 800;
    gridHeight = 600;
    processType = 0;

    initGridLimits();
    initCells();
}

/*
This function refreshes the entire display by clearing the window,
drawing Sprites determined by game state, and finally displaying all Sprites.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::refreshDisplay()
{
    // clear the window
    this->clear();

    // Draw cells
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[i].size(); j++)
        {
            if (cells[i][j].isAlive())
            {
                this->draw(cells[i][j]);
            }
        }
    }

    // Display updated frame
    this->display();
}

/*
This function updates position of all objects currently in game.
Calls several helper functions to keep the code looking a little cleaner.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::updateScene()
{
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[i].size(); j++)
        {
            updateCell(cells[i][j]);
        }
    }
}

/*
This function returns the size of the game window.

Arguments:
    N/A

Return Values:
    Vector2u - size of window
*/
Vector2u Cell_Window::getWindowSize()
{
    return windowSize;
}

void Cell_Window::initGridLimits()
{
    numCellsRow = windowSize.x / cellSize;
    numCellsCol = windowSize.y / cellSize;
    numCells = numCellsRow * numCellsCol;
}

void Cell_Window::initCells()
{
    for (int i = 0; i < numCellsRow; i++)
    {
        vector<Cell> cellRow;
        for (int j = 0; j < numCellsCol; j++)
        {
            int x = i * cellSize;
            int y = j * cellSize;
            cellRow.emplace_back(cellSize, x, y);

            uniform_int_distribution<int> distribution(0, 1);
            bool aliveOrDead = distribution(generator);
            if (aliveOrDead)
            {
                cellRow.back().setAlive();
            }
        }
        cells.push_back(cellRow);
    }
}

void Cell_Window::updateCell(Cell& cell)
{
    if (cell.isAlive())
    {
        cell.setDead();
    }
    else
    {
        cell.setAlive();
    }
}
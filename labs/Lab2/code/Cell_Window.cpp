/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 09/21/2025

Description:

Lab 2

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
            // draw live cells
            // also, update table now that all cells have been set
            if (cells[i][j].isAlive())
            {
                this->draw(cells[i][j]);
                cellStateTable[i][j] = true;
            }
            else
            {
                cellStateTable[i][j] = false;
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
            updateCell(cells[i][j], i, j);
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
    // iterate through cell rows
    for (int i = 0; i < numCellsRow; i++)
    {
        vector<Cell> cellRow; // row for cell table
        vector<bool> cellStateRow; // row for cell state table

        // iterate through cell columns in current row
        for (int j = 0; j < numCellsCol; j++)
        {
            cellRow.emplace_back(cellSize, i, j);

            uniform_int_distribution<int> distribution(0, 1);
            bool aliveOrDead = distribution(generator);
            if (aliveOrDead)
            {
                cellRow.back().setAlive();
                cellStateRow.emplace_back(true);
            }
            else
            {
                cellRow.back().setDead();
                cellStateRow.emplace_back(false);
            }
        }
        cells.push_back(cellRow);
        cellStateTable.push_back(cellStateRow);
    }
}

void Cell_Window::updateCell(Cell& cell, int row, int column)
{
    // Count live neighbors
    int liveNeighbors = checkNeighbors(row, column);
    // Apply Conway's game of life ruleset
    if (cell.isAlive())
    {
        if (liveNeighbors < 2 || liveNeighbors > 3)
        {
            cell.setDead();
        }
    }
    else
    {
        if (liveNeighbors == 3)
        {
            cell.setAlive();
        }
    }
}

int Cell_Window::checkNeighbors(int row, int column)
{
    int liveNeighbors = 0;
    liveNeighbors += checkNeighborsAbove(row, column); // as above
    liveNeighbors += checkNeighborsBelow(row, column); // so below
    liveNeighbors += checkNeighborsSide(row, column); // and check the sides, too
    return liveNeighbors;
}

int Cell_Window::checkNeighborsAbove(int row, int column)
{
    int liveNeighbors = 0;
    // All dead if we're at the top of the grid
    if (row == 0)
    {
        return 0;
    }
    // Check top left
    if (column != 0 && cellStateTable[row-1][column-1])
    {
        liveNeighbors += 1;
    }
    // Check top middle
    if (cellStateTable[row-1][column])
    {
        liveNeighbors += 1;
    }
    // Check top right
    if (column != gridWidth && cellStateTable[row-1][column+1])
    {
        liveNeighbors += 1;
    }

    return liveNeighbors;
}

int Cell_Window::checkNeighborsBelow(int row, int column)
{
    int liveNeighbors = 0;
    // All dead if we're at the bottom of the grid
    if (row == gridHeight)
    {
        return 0;
    }
    // Check bottom left
    if (column != 0 && cellStateTable[row+1][column-1])
    {
        liveNeighbors += 1;
    }
    // Check bottom middle
    if (cellStateTable[row+1][column])
    {
        liveNeighbors += 1;
    }
    // Check bottom right
    if (column != gridWidth && cellStateTable[row+1][column+1])
    {
        liveNeighbors += 1;
    }
    
    return liveNeighbors;
}

int Cell_Window::checkNeighborsSide(int row, int column)
{
    int liveNeighbors = 0;
    // Check left
    if (column != 0 && cellStateTable[row][column-1])
    {
        liveNeighbors += 1;
    }
    // Check right
    if (column != gridWidth && cellStateTable[row][column+1])
    {
        liveNeighbors += 1;
    }
    
    return liveNeighbors;
}
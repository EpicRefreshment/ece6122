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
    processType = 0;

    generationCount = 0;
    generationTime = microseconds(0);

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
                cellStateTable1[i][j] = true;
            }
            else
            {
                cellStateTable1[i][j] = false;
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
    generationCount += 1;
    generationClock.restart();

    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[i].size(); j++)
        {
            //if (i > 0 && i < (gridHeight - 1) && j > 0)
            //updateCell(cells[i][j], i, j);
        }
    }

    generationTime += generationClock.getElapsedTime();

    if (generationCount == 100)
    {
        cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with single thread." << endl;
        generationCount = 0;
        generationTime = microseconds(0);
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
    gridWidth = static_cast<int>(windowSize.x / cellSize);
    gridHeight = static_cast<int>(windowSize.y / cellSize);
    cout << "Window Resolution: " << windowSize.x << "x" << windowSize.y << endl;
    cout << "Grid Size: " << gridWidth << "x" << gridHeight << endl;
}

void Cell_Window::initCells()
{
    auto seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(static_cast<unsigned>(seed));

    uniform_int_distribution<int> distribution(0, 1);
    // iterate through cell rows
    for (int i = 0; i < gridHeight; i++)
    {
        vector<Cell> cellRow; // row for cell table
        vector<bool> cellStateRow; // row for cell state table

        // iterate through cell columns in current row
        for (int j = 0; j < gridWidth; j++)
        {
            cellRow.emplace_back(cellSize, i, j);

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
        cellStateTable1.push_back(cellStateRow);
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
    //cout << "Checking neighbors: " << endl;
    int liveNeighbors = 0;

     // All dead if we're at the top of the grid
    if (row == 0 || row == (gridHeight - 1))
    {
        return 0;
    }
    // Check above
    if (row != 0) // as above
    {
        // top center
        if (cellStateTable1[row-1][column])
        {
            liveNeighbors += 1;
        }
        // top left
        if (column != 0 && cellStateTable1[row-1][column-1])
        {
            liveNeighbors += 1;
        }
        // top right
        if (column != (gridWidth - 1) && cellStateTable1[row-1][column+1])
        {
            liveNeighbors += 1;
        }
    }
    // Check below
    if (row != (gridHeight - 1)) // so below
    {
        // bottom center
        if (cellStateTable1[row+1][column])
        {
            liveNeighbors += 1;
        }
        // bottom left
        if (column != 0 && cellStateTable1[row+1][column-1])
        {
            liveNeighbors += 1;
        }
        // bottom right
        if (column != (gridWidth - 1) && cellStateTable1[row+1][column+1])
        {
            liveNeighbors += 1;
        }
    }
    // check to the left then check to the right
    if (column != 0)
    {
        if (cellStateTable1[row][column-1])
        {
            liveNeighbors += 1;
        }
    }
    // Check right
    if (column < (gridWidth - 1))
    {
        if (cellStateTable1[row][column+1])
        {
            liveNeighbors += 1;
        }
    }
    //liveNeighbors += checkNeighborsAbove(row, column); // as above
    //liveNeighbors += checkNeighborsBelow(row, column); // so below
    //liveNeighbors += checkNeighborsSide(row, column); // and check the sides, too
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
    if (column != 0)
    {
        //cout << "1";
        if (cellStateTable1[row-1][column-1])
        {
            liveNeighbors += 1;
        }
    }
    // Check top middle
    //cout << "2";
    if (cellStateTable1[row-1][column])
    {
        liveNeighbors += 1;
    }
    // Check top right
    if (column < (gridWidth - 1))
    {
        //cout << "3";
        if (cellStateTable1[row-1][column+1])
        {
            liveNeighbors += 1;
        }
    }

    return liveNeighbors;
}

int Cell_Window::checkNeighborsBelow(int row, int column)
{
    int liveNeighbors = 0;
    // All dead if we're at the bottom of the grid
    if (row == (gridHeight - 1))
    {
        return 0;
    }
    // Check bottom left
    if (column != 0)
    {
        //cout << "4";
        if (cellStateTable1[row+1][column-1])
        {
            liveNeighbors += 1;
        }
    }
    // Check bottom middle
    //cout << "5";
    if (cellStateTable1[row+1][column])
    {
        liveNeighbors += 1;
    }
    // Check bottom right
    if (column < (gridWidth - 1))
    {
        //cout << "6";
        if (cellStateTable1[row+1][column+1])
        {
            liveNeighbors += 1;
        }
    }
    
    return liveNeighbors;
}

int Cell_Window::checkNeighborsSide(int row, int column)
{
    int liveNeighbors = 0;
    // Check left
    if (column != 0)
    {
        //cout << "7";
        if (cellStateTable1[row][column-1])
        {
            liveNeighbors += 1;
        }
    }
    // Check right
    if (column < (gridWidth - 1))
    {
        //cout << "8";
        if (cellStateTable1[row][column+1])
        {
            liveNeighbors += 1;
        }
    }
    //cout << endl;
    return liveNeighbors;
}
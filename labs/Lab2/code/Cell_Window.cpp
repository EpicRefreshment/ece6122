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
    vm = VideoMode(1200, 800); // Set window size
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
    for (int i = 1; i <= gridHeight; i++)
    {
        for (int j = 1; j <= gridWidth; j++)
        {
            // draw live cells
            if (cellStateTable[i][j] == 1)
            {
                this->draw(cells[i-1][j-1]);
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
    generationCount++;
    generationClock.restart();

    for (int i = 1; i <= gridHeight; i++)
    {
        for (int j = 1; j <= gridWidth; j++)
        {
            liveNeighbors = 0; // reset
            // Check neighbors above
            liveNeighbors += cellStateTable[i-1][j-1] +
                             cellStateTable[i-1][j] +
                             cellStateTable[i-1][j+1]; // as above
            // Check neighbors below
            liveNeighbors += cellStateTable[i+1][j-1] +
                             cellStateTable[i+1][j] +
                             cellStateTable[i+1][j+1]; // so below
            // Check neighbors to the side
            liveNeighbors += cellStateTable[i][j-1] +
                             cellStateTable[i][j+1];

            if (cellStateTableUpdated[i][j] == 1 && (liveNeighbors < 2 || liveNeighbors > 3))
            {
                cellStateTableUpdated[i][j] = 0;
            }
            else if (cellStateTableUpdated[i][j] == 0 && liveNeighbors == 3)
            {
                cellStateTableUpdated[i][j] = 1;
            }
        }
    }

    cellStateTable = cellStateTableUpdated;

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
    //cout << "Window Resolution: " << windowSize.x << "x" << windowSize.y << endl;
    //cout << "Grid Size: " << gridWidth << "x" << gridHeight << endl;
}

void Cell_Window::initCells()
{
    auto seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(static_cast<unsigned>(seed));

    uniform_int_distribution<int> distribution(0, 1);
    // iterate through cell rows
    for (int i = 0; i <= (gridHeight + 1); i++)
    {
        vector<Cell> cellRow; // row for cell table
        vector<int> cellStateRow; // row for cell state table

        // iterate through cell columns in current row
        for (int j = 0; j <= (gridWidth + 1); j++)
        {
            if (i > 0 && i <= gridHeight && j > 0 && j <= gridWidth)
            {
                cellRow.emplace_back(cellSize, i-1, j-1);

                bool aliveOrDead = distribution(generator);
                if (aliveOrDead)
                {
                    cellStateRow.emplace_back(1);
                }
                else
                {
                    cellStateRow.emplace_back(0);
                }
            }
            else
            {
                cellStateRow.emplace_back(0);
            }
        }
        if (i > 0 && i <= gridHeight)
        {
            cells.push_back(cellRow);
        }
        cellStateTable.push_back(cellStateRow);
    }
    cellStateTableUpdated = cellStateTable;
}
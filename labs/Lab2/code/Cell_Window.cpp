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
Cell_Window::Cell_Window(int width, int height, int cellSize, int numThreads, int processType)
{
    vm = VideoMode(width, height); // Set window size
    this->create(vm, "Cellular Automata", Style::Default); // Create the window

    windowSize = this->getSize(); // Get the actual window size

    this->numThreads = numThreads;

    this->cellSize = static_cast<float>(cellSize);

    generationCount = 0;
    generationTime = microseconds(0);

    this->processType = processType;
    switch (processType)
    {
        case 1:
            updateFunction = bind(&Cell_Window::updateSequential, this);
            break;
        case 2:
            updateFunction = bind(&Cell_Window::updateThreading, this);
            break;
        case 3:
            updateFunction = bind(&Cell_Window::updateMultiprocessing, this);
            break;
        default: // good habit to keep a default statement despite validating input
            updateFunction = bind(&Cell_Window::updateSequential, this);
            break;
    }

    initGridLimits();
    initCells();

    if (processType == 2 || processType == 3)
    {
        initThreads();
    }
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

    updateFunction();

    cellStateTable = cellStateTableUpdated;

    generationTime += generationClock.getElapsedTime();

    if (generationCount == 100)
    {
        outputTiming();
        generationCount = 0;
        generationTime = microseconds(0);
    }
}

void Cell_Window::outputTiming()
{
    switch (processType)
    {
        case 1:
            cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with single thread." << endl;
            break;
        case 2:
            cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with " << numThreads << " std::threads." << endl;
            break;
        case 3:
            cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with " << numThreads << " OMP threads." << endl;
            break;
        default: // good habit to keep a default statement despite validating input
            cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with single thread." << endl;
            break;
    }
}

void Cell_Window::updateSequential()
{
    for (int i = 1; i <= gridHeight; i++)
    {
        updateRow(i);
    }
}

void Cell_Window::updateThreading()
{
    cellThreads.clear();
    cellThreads.reserve(numThreads);

    for (const auto& workRows : workThreadRows)
    {
        int startRow = workRows.first;
        int endRow = workRows.second;
        cellThreads.emplace_back([this, startRow, endRow] { this->updateRows(startRow, endRow); });
    }

    for (auto& cellThread: cellThreads)
    {
        if (cellThread.joinable())
        {
            cellThread.join();
        }
    }
}

void Cell_Window::updateMultiprocessing()
{
    // This directive creates a team of threads and distributes the loop iterations.
    // Each thread will process one of the pre-calculated work chunks from the vector.
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < workThreadRows.size(); ++i)
    {
        // Use .first and .second for direct access, avoiding template issues
        int startRow = workThreadRows[i].first;
        int endRow = workThreadRows[i].second;
        updateRows(startRow, endRow);
    }
}

void Cell_Window::updateRow(int row)
{
    for (int j = 1; j <= gridWidth; j++)
    {
        int liveNeighbors = 0; // reset
        // Check neighbors above
        liveNeighbors += cellStateTable[row-1][j-1] +
                         cellStateTable[row-1][j] +
                         cellStateTable[row-1][j+1]; // as above
        // Check neighbors below
        liveNeighbors += cellStateTable[row+1][j-1] +
                         cellStateTable[row+1][j] +
                         cellStateTable[row+1][j+1]; // so below
        // Check neighbors to the side
        liveNeighbors += cellStateTable[row][j-1] +
                         cellStateTable[row][j+1];

        int isAlive = cellStateTable[row][j];
        cellStateTableUpdated[row][j] = (liveNeighbors == 3) ||
                                      (isAlive && liveNeighbors == 2);
    }
}
void Cell_Window::updateRows(int start, int end)
{
    for (int i = start; i < end; i++)
    {
        for (int j = 1; j <= gridWidth; j++)
        {
            int liveNeighbors = 0; // reset
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

            int isAlive = cellStateTable[i][j];
            cellStateTableUpdated[i][j] = (liveNeighbors == 3) ||
                                          (isAlive && liveNeighbors == 2);
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
    gridWidth = static_cast<int>(windowSize.x / cellSize);
    gridHeight = static_cast<int>(windowSize.y / cellSize);
    cout << "Window Resolution: " << windowSize.x << "x" << windowSize.y << endl;
    cout << "Grid Size: " << gridWidth << "x" << gridHeight << endl;
}

void Cell_Window::initCells()
{
    // Establish random generator for initializing cell states
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
                cellRow.emplace_back(cellSize, i-1, j-1); // create a new Cell at the back of the cell vector

                bool aliveOrDead = distribution(generator); // assign random state to cell
                if (aliveOrDead) // It's alive
                {
                    cellStateRow.emplace_back(1);
                }
                else // It's dead
                {
                    cellStateRow.emplace_back(0);
                }
            }
            else // add false cells surrounding the cellular automata grid to act as the edge
            {
                cellStateRow.emplace_back(0);
            }
        }

        // add row to top level vector
        if (i > 0 && i <= gridHeight) // ignore the bounding false rows
        {
            cells.push_back(cellRow);
        }
        cellStateTable.push_back(cellStateRow);
    }
    cellStateTableUpdated = cellStateTable; // copy cell state table into the "update" table
}

void Cell_Window::initThreads()
{
    if (numThreads > gridHeight)
    {
        numThreads = gridHeight;
    }
    perThreadRows = gridHeight / numThreads;
    remainingRows = gridHeight % numThreads;
    cout << "Rows per thread: " << perThreadRows << " Row remainder: " << remainingRows << endl;

    // initialize variables for start and end row
    // set to 1 for each to account for the false cell edges
    int rowStart = 1;
    int rowEnd = 1;

    for (int i = 0; i < numThreads; i++)
    {
        rowStart = rowEnd; // set to end of last section
        rowEnd = rowStart + perThreadRows; // add initial calculation of rows per thread

        // evenly distribute remaining rows
        // to equalize distributions as much as possible
        if (i < remainingRows)
        {
            rowEnd++;
        }

        // add tuple to vector of start and end rows
        workThreadRows.emplace_back(rowStart, rowEnd);
        cout << "Thread " << (i + 1) << ": Start: " << rowStart << " End: " << rowEnd << " Rows: " << (rowEnd - rowStart) << endl;
    }
}
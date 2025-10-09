/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 10/08/2025

Description:

Lab 2

This is the source file for the Cell_Window class and implements all functions
and maintains all variables defined in Cell_Window.h.
This class is derived from the SFML Window class.
Cell_Window handles all logic to simulate Conway's Game of Life and 
displaying visual results and timing results to console. It has three separate
update functions for cells to support three methods of calculating each new generation:
sequential (single thread), multithreading, and multiprocessing.

*/

#include "Cell_Window.h"

using namespace sf;
using namespace std;

/*
This is the constructor for Cell_Window. This function initializes the window
and initializes the cell grid. Calls other helper functions to initialize
different aspects of the simulation.

Arguments:
    width - width of window and used to determine grid size
    height - height of window and used to determine grid size
    cellSize - size of cell
    numThreads - number of threads to use. Ignored if using single thread.
    proccessType - calculation method used in simulation

Return Values:
    Cell_Window
*/
Cell_Window::Cell_Window(int width, int height, int cellSize, int numThreads, int processType, int debug)
{
    this->debug = debug;

    vm = VideoMode(width, height); // Set window size
    this->create(vm, "Cellular Automata", Style::Default); // Create the window

    windowSize = this->getSize(); // Get the actual window size

    this->numThreads = numThreads;

    this->cellSize = static_cast<float>(cellSize);

    // initialize timing variables
    generationCount = 0;
    generationTime = microseconds(0);

    // call init helper functions
    initGridLimits();
    initCells();

    // determine method of processing the grid accordingly
    // call additional init helper functions if necessary
    int maxHWThreads = omp_get_max_threads();
    this->processType = processType;
    switch (processType)
    {
        case 1: // SEQ, sequential/single thread
            updateFunction = bind(&Cell_Window::updateSequential, this);
            break;
        case 2: // THRD, multithreading
            updateFunction = bind(&Cell_Window::updateThreading, this);
            initThreads();
            assignWork();
            break;
        case 3: // OMP, multiprocessing
            // check we're not going over hardware limit
            if (this->numThreads > maxHWThreads)
            {
                this->numThreads = maxHWThreads; // just use max available if so.
                cout << "Max Hardware Threads Exceeded. Limit: " << maxHWThreads << endl;
                cout << "Using max available threads instead." << endl;
            }
            updateFunction = bind(&Cell_Window::updateMultiprocessing, this);
            assignWork();
            break;
        default: // good habit to keep a default statement despite validating input
            updateFunction = bind(&Cell_Window::updateSequential, this);
            break;
    }
}

/*
This is the deconstructor for Cell_Window. 
This function exists to close all threads gracefully when
program is closed

Arguments:
    N/A

Return Values:
    N/A
*/
Cell_Window::~Cell_Window()
{
    stop_threads = true;
    condition.notify_all(); // Wake up all threads so they can check the stop flag and exit
    for (thread& t : cellThreads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}

/*
This function refreshes the entire display by clearing the window,
drawing cells if they are alive, and finally displaying the new state of the grid.

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
    for (int i = 1; i <= gridHeight; i++) // row
    {
        for (int j = 1; j <= gridWidth; j++) // grid
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
This function updates state of all cells.
Calls update function that points to one of three update functions:
updateSequential, updateThreading, updateMultiprocessing.
Also calculates time it took to process the current generation and outputs
timing results for every 100 generations.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::updateScene()
{
    generationCount++;
    generationClock.restart(); // start timer for cell processing

    updateFunction(); // update all cells

    cellStateTable = cellStateTableUpdated; // copy updated states into current state

    generationTime += generationClock.getElapsedTime(); // stop timer and add to current processing time

    if (generationCount == 100) // wait for 100 generations to be processed
    {
        outputTiming(); // output timing results
        generationCount = 0; // reset count
        generationTime = microseconds(0); // reset processing time
    }
}

/*
This function outputs timing results to console.
Output depends on processType

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::outputTiming()
{
    switch (processType)
    {
        case 1: // SEQ
            cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with single thread." << endl;
            break;
        case 2: // THRD
            cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with " << numThreads << " std::threads." << endl;
            break;
        case 3: // OMP
            cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with " << numThreads << " OMP threads." << endl;
            break;
        default: // good habit to keep a default statement despite validating input
            cout << "100 generations took " << generationTime.asMicroseconds() << " microseconds with single thread." << endl;
            break;
    }
}

/*
This function updates all cells in grid sequentially.
Calls helper function for each row in grid.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::updateSequential()
{
    for (int i = 1; i <= gridHeight; i++)
    {
        updateRow(i);
    }
}

/*
This function updates all cells in grid using multiple threads.
Adds task to queue for threads to process and uses vector of pairs
to assign a subset of rows to that particular task.
The task is a helper function to update a subset of rows in the grid.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::updateThreading()
{
   { // Use a scoped lock to safely add tasks to the queue
        unique_lock<mutex> lock(queueMutex);

        // Set the counter for how many tasks need to complete this frame
        active_tasks = static_cast<int>(workThreadRows.size());

        // Add a task for each work chunk to the queue
        for (const auto& workRows : workThreadRows)
        {
            tasks.push([this, workRows] {
                const auto [startRow, endRow] = workRows;
                this->updateRows(startRow, endRow);
            });
        }
    } // Lock is released here

    // Wake up all sleeping worker threads to start processing tasks
    condition.notify_all();

    // Wait until all tasks for this frame are complete
    while (active_tasks > 0)
    {
        this_thread::yield(); // Yield to prevent the main thread from busy-waiting
    }
}

/*
This function updates all cells in grid using multiprocessing.
Calls helper function for a subset of rows in grid.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::updateMultiprocessing()
{
    // This directive creates a team of threads and distributes the loop iterations.
    // Each thread will process one of the pre-calculated work chunks from the vector.
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < workThreadRows.size(); i++)
    {
        // Use .first and .second for direct access, avoiding template issues
        int startRow = workThreadRows[i].first;
        int endRow = workThreadRows[i].second;
        updateRows(startRow, endRow);
    }
}

/*
This function updates all cells in a given row by
iterating through each cell, summing all neighboring cell states,
then determining the state of that cell based on the rules of Conway's Game of Life.
Reduces rules to one if statement, and takes advantage of a dead cell boundary to avoid
numerous branches to check for nonexistent cells.

Arguments:
    row - a row in the grid to process

Return Values:
    void
*/
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

/*
This function operates similarly to updateRow(), but processes a set
of rows.

Arguments:
    start - the first row in the grid to process
    end - the last row in the grid to process

Return Values:
    void
*/
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

/*
This function determines the size of cell grid based on the 
given window size and cell size.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::initGridLimits()
{
    gridWidth = static_cast<int>(windowSize.x / cellSize);
    gridHeight = static_cast<int>(windowSize.y / cellSize);
    // remaining pixels if grid size is not evenly divided by cell size
    remainingPixelsWidth = windowSize.x % static_cast<int>(cellSize);
    remainingPixelsHeight = windowSize.y % static_cast<int>(cellSize);
    if (debug)
    {
        cout << "Window Resolution: " << windowSize.x << "x" << windowSize.y << endl;
        cout << "Grid Size: " << gridWidth << "x" << gridHeight << endl;
        cout << "X Offset: " << remainingPixelsWidth / 2.0f << endl;
        cout << "Y Offset: " << remainingPixelsHeight / 2.0f << endl;
    }
}

/*
This function initializes all the cells in the grid. It uses the grid limits 
calculated prior to determine the number of cells in a row and the number of rows.
Uses a random number generator to give the cell a state to start with: either alive or dead.
It manages three 2D vectors, one of which contains Cell objects for displaying on the grid, 
and two that contain the state of each cell as an int: 0 (dead) or 1 (alive).
One is for maintaining current state of cells for update purposes and one to store updated state. 
The updated state vector is routinely copied into the other once all processing is done.
It adds an extra row of cells above and below the grid (and on either side of each row) to act
as a dead cell boundary to simplify cell state calculations. 

Arguments:
    N/A

Return Values:
    void
*/
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
                // create a new Cell at the back of the cell vector
                cellRow.emplace_back(cellSize, i-1, j-1, remainingPixelsWidth, remainingPixelsHeight);

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

        // add rows to top level vector
        if (i > 0 && i <= gridHeight) // ignore the bounding false rows
        {
            cells.push_back(cellRow);
        }
        cellStateTable.push_back(cellStateRow);
    }
    cellStateTableUpdated = cellStateTable; // copy cell state table into the "update" table
}

/*
This function initializes a specified number of threads each bound to a cellWorker function
and adds them to a vector of threads.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::initThreads()
{
    for (int i = 0; i < numThreads; i++)
    {
        cellThreads.emplace_back(&Cell_Window::cellWorker, this);
    }
}

/*
This function acts as the main loop for each thread. It waits for a task to be queued, 
then executes that task if currently available.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::cellWorker()
{
    while (true)
    {
        Task task;
        {   // Lock the queue to safely grab a task
            unique_lock<mutex> lock(queueMutex);

            // Wait until there's a task or the pool is stopping
            condition.wait(lock, [this] { 
                return !tasks.empty() || stop_threads; 
            });

            if (stop_threads && tasks.empty()) {
                return; // Exit loop if stopped and no tasks left
            }

            task = move(tasks.front());
            tasks.pop();
        } // Lock is released here

        task(); // Execute the task
        active_tasks--; // Decrement the task counter to signal completion
    }
}

/*
This function divides the grid by number of threads as evenly as possible
into subsets of rows for each thread (or more accurately task) to process.
This is used for both multithreading and multiprocessing.

Arguments:
    N/A

Return Values:
    void
*/
void Cell_Window::assignWork()
{
    if (numThreads > gridHeight) // if we have more threads than rows to process.
    {
        numThreads = gridHeight;
    }
    perThreadRows = gridHeight / numThreads; // divide by grid by number of threads.
    remainingRows = gridHeight % numThreads; // calculate remaining rows if unevenly divided

    if (debug)
    {
        cout << "Rows per thread: " << perThreadRows << " Row remainder: " << remainingRows << endl;
    }

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

        // add pair to vector of start and end rows
        workThreadRows.emplace_back(rowStart, rowEnd);
        if (debug)
        {
            cout << "Thread " << (i + 1) << ": Start: " << rowStart << ", End: " << rowEnd << ", Total rows: " << (rowEnd - rowStart) << endl;
        }
    }
}
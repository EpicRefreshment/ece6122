/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 10/08/2025

Description:

Lab 2

This is the header file for the Cell_Window class
This class is derived from the SFML Window class.
Cell_Window handles all logic to simulate Conway's Game of Life and 
displaying visual results and timing results to console. It has three separate
update functions for cells to support three methods of calculating each new generation:
sequential (single thread), multithreading, and multiprocessing.

*/

#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <utility>

#include <omp.h>

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include <Cell.h>

using namespace sf;
using namespace std;

class Cell_Window: public RenderWindow
{
public:
    /***************************
      Public member functions
    ****************************/

    // Constructor
    Cell_Window(int width, int height, int cellSize, int numThreads, int processType, int debug);
    //Destructor
    ~Cell_Window();

    // Manage window and objects
    void refreshDisplay(); // Clears and redraws all appropriate game objects
    void updateScene(); // main function for updating all game objects

    Vector2u getWindowSize(); // Returns size of game window
private:
    /***************************
      Private member functions
    ****************************/

    // functions to initialize simulation
    void initGridLimits(); // initializes grid size
    void initCells(); // initializes cells in grid
    void initThreads(); // initializes thread objects for multithreading

    // functions to update cells using various processing methods
    void updateSequential(); // single threaded
    void updateThreading(); // multithreading
    void updateMultiprocessing(); // multiprocessing

    // functions to update rows of cells in the grid
    void updateRow(int row); // updates one row
    void updateRows(int start, int end); // updates a range of rows

    // helper functions multithreading/processing
    void cellWorker(); // monitors for tasks to complete. bound by a thread
    void assignWork(); // divides the processing tasks evenly between all threads/cores

    // misc helper functions
    void outputTiming(); // outputs timing results to console

    /***************************
      Private member variables
    ****************************/
    
    int debug; // controls output meant for debugging project

    // Variables for main game window
    VideoMode vm;
    Vector2u windowSize;

    // Full screen textures and sizes
    Texture backgroundTexture;

    // Full screen sprites
    Sprite backgroundSprite;

    // Texture Size
    Vector2u backgroundSize;

    // cellular automata parameters
    // can be set using command line arguments
    int gridWidth;
    int gridHeight;
    float cellSize;
    int numThreads;
    int processType;

    // remaining pixels if grid size is not evenly divided by cell size
    int remainingPixelsWidth;
    int remainingPixelsHeight;

    // function pointer for appropriate update function
    // depending on processType
    function<void()> updateFunction;

    // cell/cell state vectors
    vector<vector<Cell>> cells;
    vector<vector<int>> cellStateTable;
    vector<vector<int>> cellStateTableUpdated;

    // threading variables
    vector<thread> cellThreads;
    vector<pair<int, int>> workThreadRows;
    int perThreadRows;
    int remainingRows;

    // Thread pool variables
    using Task = function<void()>;
    queue<Task> tasks;
    mutex queueMutex;
    condition_variable condition;
    atomic<bool> stop_threads{false};
    atomic<int> active_tasks{0};

    // for tracking cell generation processing time
    Clock generationClock;
    Time generationTime;
    int generationCount;
};
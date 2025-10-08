/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the header file for the ECE_Defender class.
This class is derived from the SFML Window class.
ECE_Defender manages all game objects, state, and logic. 

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
    Cell_Window(int width, int height, int cellSize, int numThreads, int processType);
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
    void parseArguments();
    void initGridLimits();
    void initCells();
    void initThreads();

    void updateSequential();
    void updateThreading();
    void updateMultiprocessing();

    void updateRow(int row);
    void updateRows(int start, int end);

    void cellWorker();
    void assignWork();

    void outputTiming();

    /***************************
      Private member variables
    ****************************/

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
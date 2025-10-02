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

#include <list>
#include <random>
#include <iostream>

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
    Cell_Window();

    // Manage window and objects
    void refreshDisplay(); // Clears and redraws all appropriate game objects
    void updateScene(); // main function for updating all game objects

    Vector2u getWindowSize(); // Returns size of game window
private:
    /***************************
      Private member functions
    ****************************/

    void updateCell(Cell& cell, int row, int column);
    int checkNeighbors(int row, int column);
    int checkNeighborsAbove(int row, int column);
    int checkNeighborsBelow(int row, int column);
    int checkNeighborsSide(int row, int column);

    void initGridLimits();
    void initCells();

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

    int numThreads;
    float cellSize;
    int gridWidth;
    int gridHeight;
    int processType;

    int numCells;
    int numCellsRow;
    int numCellsCol;

    vector<vector<Cell>> cells;
    vector<vector<bool>> cellStateTable;

    default_random_engine generator;
};
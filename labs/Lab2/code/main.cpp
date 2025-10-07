/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 09/21/2025

Description:

Lab 2

This is the main file for the conway's game of life simulation.
This handles simple overview of handling window 

*/

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <vector>

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "Cell_Window.h"
#include "Command_Parser.h"

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

int main(int argc, char* argv[])
{
	// parse arguments
	int width = 800;
	int height = 600;
	int cellSize = 5;
	int numThreads = 8;
	int processType = 2;

	// make sure expected number of arguments exist
	if (argc == 2 || argc == 4 || argc == 6 || argc == 8 || argc > 9)
	{
		cout << "Invalid input!" << endl;
		return 0;
	}

	for (int i = 1; i < argc; i++)
	{
		string command = argv[i++];
		if (command == "-x")
		{
			width = parseWidth(argv[i]);
			if (width < 0)
			{
				cout << "Invalid width." << endl;
				return 0;
			}
		}
		else if (command == "-y")
		{
			height = parseHeight(argv[i]);
			if (height < 0)
			{
				cout << "Invalid height." << endl;
				return 0;
			}
		}
		else if (command == "-c")
		{
			cellSize = parseCellSize(argv[i], width, height);
			if (cellSize < 0)
			{
				cout << "Invalid cell size." << endl;
				return 0;
			}
		}
		else if (command == "-n")
		{
			numThreads = parseNumThreads(argv[i]);
			if (numThreads < 0)
			{
				cout << "Invalid number of threads." << endl;
				return 0;
			}
		}
		else if (command == "-t")
		{
			processType = parseProcessType(argv[i]);
			if (processType < 0)
			{
				cout << "Invalid process type." << endl;
				return 0;
			}
		}
		else
		{
			cout << "Invalid command." << endl;
			return 0;
		}
	}

	// Create object to manage the game window and overall game state
	// This object exists mainly to keep 'main' clean
	Cell_Window cellularAutomata(width, height, cellSize, numThreads, static_cast<int>(processType));
	cellularAutomata.refreshDisplay();

	while (cellularAutomata.isOpen())
	{
		Event event;
		while (cellularAutomata.pollEvent(event)) // Handle mouse events
		{
			// Close if user clicks the close button
			if (event.type == Event::Closed)
			{
				cellularAutomata.close();
			}
		}

		// Close simulation if Escape is pressed
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			cellularAutomata.close();
		}

		cellularAutomata.updateScene();
		// Clear and redraw the window with updated scene
		cellularAutomata.refreshDisplay();
		// sleep so display doesn't refresh too fast
		//this_thread::sleep_for(chrono::milliseconds(20));
	}

	return 0;
}
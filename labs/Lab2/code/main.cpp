/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 10/08/2025

Description:

Lab 2

This is the main file for the conway's game of life simulation.
This handles simple overview of handling window and parses all optional command
line arguments.

Arguments:
-x: width of window and cell grid, default = 800
-y: height of window and cell grid, default = 600
-c: cell size, default = 5
-n: number of threads, default = 8
-t: processing type, default = THRD

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
	int debug = 0;
	int invalidInput = 0;

	// make sure there aren't too many arguments
	// any number <= 12 is valid
	if (argc > 12)
	{
		invalidInput = 8;
	}

	if (invalidInput == 0) // don/t bother checking arguments if wrong amount is present
	{	
		for (int i = 1; i < argc; i++)
		{
			string command = argv[i++];
			if (command == "-x") // window width
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 9;
					break;
				}

				width = parseWidth(argv[i]);

				if (width < 0) // validate
				{
					invalidInput = 1;
				}
			}
			else if (command == "-y") // window height
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 9;
					break;
				}

				height = parseHeight(argv[i]);

				if (height < 0) // validate
				{
					invalidInput = 2;
				}
			}
			else if (command == "-c") // cell size
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 9;
					break;
				}

				cellSize = parseCellSize(argv[i], width, height);

				if (cellSize < 0) // validate
				{
					invalidInput = 3;
				}
			}
			else if (command == "-n") // number of threads
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 9;
					break;
				}

				numThreads = parseNumThreads(argv[i]);

				if (numThreads < 0) // validate
				{
					invalidInput = 4;
				}
			}
			else if (command == "-t") // processing type
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 9;
					break;
				}

				processType = parseProcessType(argv[i]);

				if (processType < 0) // validate
				{
					invalidInput = 5;
				}
				else if (processType == 3 && numThreads > 16) // expected max cores is 16
				{
					invalidInput = 6;
				}
			}
			else if (command == "--debug" || command == "-d") // enable debug message output
			{
				debug = 1;
				cout << "Debug output enabled." << endl;
			}
			else // not a valid flag
			{
				invalidInput = 7;
			}
		}
	}

	if (invalidInput != 0) // invalid input close program.
	{
		if (debug) // debug enabled
		{
			switch (invalidInput) // output appropriate error with command line arguments
			{
				case 1:
					cout << "Invalid width." << endl;
					break;
				case 2:
					cout << "Invalid height." << endl;
					break;
				case 3:
					cout << "Invalid cell size." << endl;
					break;
				case 4:
					cout << "Invalid number of threads." << endl;
					break;
				case 5:
					cout << "Invalid process type." << endl;
					break;
				case 6:
					cout << "invalid number of threads." << endl;
					break;
				case 7:
					cout << "Invalid command." << endl;
					break;
				case 8:
					cout << "Too many arguments." << endl;
					break;
				case 9:
					cout << "Missing value." << endl;
					break;
				default:
					cout << "Invalid command." << endl;
					break;
			}
		}
		cout << "Invalid input!" << endl;
		return 0; // close!
	}

	// Create object to manage the game window and overall game state
	// This object exists mainly to keep 'main' clean
	Cell_Window cellularAutomata(width, height, cellSize, numThreads, processType, debug);
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
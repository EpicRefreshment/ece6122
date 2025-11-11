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
	// default arguments
	int width = 800;
	int height = 600;
	int cellSize = 5;
	int numThreads = 8;
	int processType = 2;

	// debug flag for extra output not specifically required by project description.
	// Don't want to lose points for all my extra debug statements!
	int debug = 0;
	// indicates to program an invalid input was detected.
	// If debug is on, the value determines the message output with more specific error description
	int invalidInput = 0; // indicates to program an invalid input was detected.

	vector<int> argRecv = {0, 0, 0, 0, 0}; // to prevent duplicate arguments

	// make sure there aren't too many arguments
	// any number <= 12 is valid
	if (argc > 12)
	{
		invalidInput = 7;
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
					invalidInput = 8;
					break;
				}
				else if (argRecv[0]) // check for duplicate command
				{
					invalidInput = 9;
				}

				width = parseWidth(argv[i]);

				if (width < 0) // validate
				{
					invalidInput = 1;
				}

				argRecv[0] = 1; // command received. Used to check for duplicate commands.
			}
			else if (command == "-y") // window height
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 8;
					break;
				}
				else if (argRecv[1]) // check for duplicate command
				{
					invalidInput = 9;
				}

				height = parseHeight(argv[i]);

				if (height < 0) // validate
				{
					invalidInput = 2;
				}

				argRecv[1] = 1; // command received. Used to check for duplicate commands.
			}
			else if (command == "-c") // cell size
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 8;
					break;
				}
				else if (argRecv[2]) // check for duplicate command
				{
					invalidInput = 9;
				}

				cellSize = parseCellSize(argv[i], width, height);

				if (cellSize < 0) // validate
				{
					invalidInput = 3;
				}
				
				argRecv[2] = 1; // command received. Used to check for duplicate commands.
			}
			else if (command == "-n") // number of threads
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 8;
					break;
				}
				else if (argRecv[3]) // check for duplicate command
				{
					invalidInput = 9;
				}

				numThreads = parseNumThreads(argv[i]);

				if (numThreads < 0) // validate
				{
					invalidInput = 4;
				}

				argRecv[3] = 1; // command received. Used to check for duplicate commands.
			}
			else if (command == "-t") // processing type
			{
				if (i == argc) // if there are no further arguments
				{
					invalidInput = 8;
					break;
				}
				else if (argRecv[4]) // check for duplicate command
				{
					invalidInput = 9;
				}

				processType = parseProcessType(argv[i]);

				if (processType < 0) // validate
				{
					invalidInput = 5;
				}

				argRecv[4] = 1; // command received. Used to check for duplicate commands.
			}
			else if (command == "--debug" || command == "-d") // enable debug message output
			{
				if (i != argc) // must be last flag
				{
					invalidInput = 10;
					// debug should be enabled even if it's not in the last position. 
					// Otherwise, you would never know that was the issue.
				}
				debug = 1; 
				cout << "Debug output enabled." << endl;
				break;
			}
			else // not a valid flag
			{
				invalidInput = 6;
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
					cout << "Invalid command." << endl;
					break;
				case 7:
					cout << "Too many arguments." << endl;
					break;
				case 8:
					cout << "Missing value." << endl;
					break;
				case 9:
					cout << "Duplicate argument." << endl;
					break;
				case 10:
					cout << "Debug flag goes at end of command arguments." << endl;
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
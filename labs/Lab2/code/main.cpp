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

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "include/Cell_Window.h"

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

int main(int argc, char* argv[])
{
	// Create object to manage the game window and overall game state
	// This object exists mainly to keep 'main' clean
	Cell_Window cellularAutomata;
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
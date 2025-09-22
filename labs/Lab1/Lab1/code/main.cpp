/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the main file for Buzzy Defender.
This handles simple overview of game state via ECE_Defender object. 

*/

// Include standard C++ libraries here
#include <iostream>

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "include/ECE_Defender.h"
#include "include/ECE_Buzzy.h"

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

int main()
{
	// Create object to manage the game window and overall game state
	// This object exists mainly to keep 'main' clean
	ECE_Defender defender;

	// Manage frame update time to keep movement smooth
	Clock frameClock;
	Time lastFrameTime = Time::Zero;
	float lag;

	while (defender.isOpen())
	{
		// Measure frame time and lag
		lastFrameTime = frameClock.restart();
		lag = lastFrameTime.asSeconds();

		while (lag > 1.0f / 60.0f)
		{
			// Close the game
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				defender.close();
			}

			// Start the game
			if (Keyboard::isKeyPressed(Keyboard::Return) && defender.isGamePaused())
			{
				defender.startGame();
			}

			// If game is over or won, pause the game to return to start screen
			if (defender.isGameOver() || defender.isGameWon())
			{
				defender.pauseGame();
			}

			// Update all game objects in scene if game is running	
			if (!defender.isGamePaused())
			{
				defender.updateScene(lastFrameTime);
			}
			lag -= 1.0f / 60.0f;
		}
		// Clear and redraw the window with updated scene
		defender.refreshDisplay();
	}

	return 0;
}



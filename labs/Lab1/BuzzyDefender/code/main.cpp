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

	// Initialize variables for game loop
	bool paused = true;
	bool acceptInput = false;

	while (defender.isOpen())
	{
		
		// Close the game
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			defender.close();
		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			acceptInput = true;
		}

		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput)
		{
			// Update all game objects in scene
			defender.updateScene();
		}

		// Clear and redraw the window with updated scene
		defender.refreshDisplay();
	}

	return 0;
}



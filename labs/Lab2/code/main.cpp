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

int main()
{
	// Create object to manage the game window and overall game state
	// This object exists mainly to keep 'main' clean
	Cell_Window cellularAutomata;
	cellularAutomata.refreshDisplay();

	while (cellularAutomata.isOpen())
	{
		// Close the game
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			cellularAutomata.close();
		}

		cellularAutomata.updateScene();
		// Clear and redraw the window with updated scene
		cellularAutomata.refreshDisplay();
		this_thread::sleep_for(chrono::milliseconds(100));
	}

	return 0;
}
// Include standard C++ libraries here
#include <iostream>

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "include/ECE_Buzzy.h"

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

int main()
{
	// Create Window
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Buzzy Defender", Style::Default);
	Vector2u windowSize = window.getSize();

	// Create background texture
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	Vector2u textureSize = textureBackground.getSize();

	// get scale of window
	float scaleX = (float) windowSize.x / textureSize.x;
	float scaleY = (float) windowSize.y / textureSize.y;
	
	// Create background sprite
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setScale(scaleX, scaleY);
	spriteBackground.setPosition(0, 0);

	// Initialize game objects
	ECE_Buzzy buzzy(windowSize);

	// Initialize variables for game loop
	bool paused = true;
	bool acceptInput = false;

	while (window.isOpen())
	{
		
		// Handle input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
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
			buzzy.update();
		}

		// Update scene

		// Draw scene

		// Refresh display
		window.clear();
		window.draw(spriteBackground);
		window.draw(buzzy.spriteBuzzy);
		window.display();
	}

	return 0;
}



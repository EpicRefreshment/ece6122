#include <iostream>

#include "ECE_Buzzy.h"

using namespace sf;
using namespace std;

ECE_Buzzy::ECE_Buzzy(Vector2u windowSize)
{
    // Load the buzzy texture from file
    if (!textureBuzzy.loadFromFile("graphics/Buzzy_blue.png"))
    {
        cout << "Error loading Buzzy.png" << endl;
    }

    // Scale buzzy to fit screen proportionally
    buzzySize = textureBuzzy.getSize();
	float scaleX = (float) windowSize.y / buzzySize.x; // Maintain aspect ratio to avoid stretching image
	float scaleY = (float) windowSize.y / buzzySize.y;

    // Create sprite
    spriteBuzzy.setTexture(textureBuzzy);
    spriteBuzzy.setScale(scaleX / 10.0f, scaleY / 10.0f); // Scale down to 1/10 size

    // Set initial position
    positionXBuzzy = (windowSize.x / 2.0f) - buzzySize.x / 20.0f; // Center horizontally
    positionYBuzzy = buzzySize.y / 20.0f; // Keep Buzzy a bit below top of window
    spriteBuzzy.setPosition(positionXBuzzy, positionYBuzzy);

    // Set speed
    speedBuzzy = 300.0f;

    // Store screen boundary
    screenBoundary = windowSize;
}

void ECE_Buzzy::update()
{
    // Move Buzzy left or right based on keyboard input
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        positionXBuzzy -= speedBuzzy * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
        if (positionXBuzzy < 0)
        {
            // Prevent moving off left edge
            positionXBuzzy = 0;
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        positionXBuzzy += speedBuzzy * 0.016f;
        if (positionXBuzzy > screenBoundary.x - buzzySize.x / 4.0f)
        { 
            // Prevent moving off right edge
            positionXBuzzy = screenBoundary.x - buzzySize.x / 4.0f;
        }
    }

    // Update sprite position
    spriteBuzzy.setPosition(positionXBuzzy, positionYBuzzy);
}
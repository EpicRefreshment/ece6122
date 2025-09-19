#include <iostream>

#include "ECE_Buzzy.h"

using namespace sf;
using namespace std;

ECE_Buzzy::ECE_Buzzy()
{
    // Load the buzzy texture from file
    buzzyTexture.loadFromFile("graphics/Buzzy_blue.png");

    // Create sprite and scale to fit screen proportionally
    this->setTexture(buzzyTexture);

    // Set speed
    buzzySpeed = 300.0f;
}

void ECE_Buzzy::scaleBuzzy(Vector2u windowSize)
{
    // Scale buzzy to fit screen proportionally
    buzzySize = buzzyTexture.getSize();
    float scaleX = (float) windowSize.y / buzzySize.x; // Maintain aspect ratio to avoid stretching image
    float scaleY = (float) windowSize.y / buzzySize.y;

    // Rescale sprite
    this->setScale(scaleX / 10.0f, scaleY / 10.0f); // Scale down to 1/10 size

    // Update screen boundary
    screenBoundary = windowSize;
}

void ECE_Buzzy::setInitialPosition()
{
    // Set initial position
    buzzyPosX = (screenBoundary.x / 2.0f) - buzzySize.x / 20.0f; // Center horizontally
    buzzyPosY = buzzySize.y / 20.0f; // Keep Buzzy a bit below top of window
    this->setPosition(buzzyPosX, buzzyPosY);
}

void ECE_Buzzy::update()
{
    // Move Buzzy left
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        buzzyPosX -= buzzySpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
        if (buzzyPosX < 0)
        {
            // Prevent moving off left edge
            buzzyPosX = 0;
        }
    }

    // Move Buzzy right
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        buzzyPosX += buzzySpeed * 0.016f;
        if (buzzyPosX > screenBoundary.x - buzzySize.x / 4.0f)
        { 
            // Prevent moving off right edge
            buzzyPosX = screenBoundary.x - buzzySize.x / 4.0f;
        }
    }

    // Update sprite position
    this->setPosition(buzzyPosX, buzzyPosY);
}
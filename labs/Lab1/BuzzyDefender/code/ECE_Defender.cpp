#include <iostream>

#include "ECE_Defender.h"

using namespace sf;
using namespace std;

ECE_Defender::ECE_Defender()
{
    vm = VideoMode(1920, 1080); // Set window size
    this->create(vm, "Buzzy Defender", Style::Default); // Create the window

    windowSize = this->getSize(); // Get the actual window size

    setupBackground(); // Setup the background

    // Initialize game objects
    // Buzzy
    buzzy.scaleBuzzy(windowSize);
    buzzy.setInitialPosition();
    // Enemies

    // Lasers
}

void ECE_Defender::setupBackground()
{
    // Load the background texture from file
    backgroundTexture.loadFromFile("graphics/background.png");
    
    // Scale background to fit screen
    backgroundSize = backgroundTexture.getSize();
    float scaleX = (float) windowSize.x / backgroundSize.x;
    float scaleY = (float) windowSize.y / backgroundSize.y;

    // Create sprite
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(scaleX, scaleY);
    backgroundSprite.setPosition(0, 0);
}

void ECE_Defender::refreshDisplay()
{
    this->clear();
    this->draw(backgroundSprite);
    this->draw(buzzy);
    this->display();
}

void ECE_Defender::updateScene()
{
    // Update game objects

    // Update buzzy based on user input
    buzzy.update();
}


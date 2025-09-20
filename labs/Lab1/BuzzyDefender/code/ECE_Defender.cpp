#include <iostream>

#include "ECE_Defender.h"

using namespace sf;
using namespace std;

ECE_Defender::ECE_Defender()
{
    vm = VideoMode(1920, 1080); // Set window size
    this->create(vm, "Buzzy Defender", Style::Default); // Create the window

    windowSize = this->getSize(); // Get the actual window size

    loadTextures(); // Load all textures
    setupBackground(); // Setup the background

    // Initialize player and enemies
    // Buzzy
    buzzy.setupBuzzy(buzzyTexture, windowSize);
    // Enemies
}

void ECE_Defender::loadTextures()
{
    // Load all textures from file
    backgroundTexture.loadFromFile("graphics/background.png");
    buzzyTexture.loadFromFile("graphics/Buzzy_blue.png");
    enemyTexture.loadFromFile("graphics/enemyRed1.png");
    playerLaserTexture.loadFromFile("graphics/player_laser.png");
    enemyLaserTexture.loadFromFile("graphics/enemy_laser.png");

}

void ECE_Defender::setupBackground()
{   
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
    for (auto laser = laserBlasts.begin(); laser != laserBlasts.end(); ++laser)
    {
        this->draw(*laser);
    }
    this->display();
}

void ECE_Defender::updateScene()
{
    this->fireCooldown = milliseconds(250); // 250 ms cooldown

    // Update buzzy based on user input
    buzzy.update();

    // Update laser blasts on the next frame after they were initialized
    for (auto laser = laserBlasts.begin(); laser != laserBlasts.end(); )
    {
        laser->update();
        if (laser->boundaryDetected())
        {
            // Remove laser if it goes out of bounds
            laser = laserBlasts.erase(laser);
        }
        else
        {
            // erase automatically advances iterator so iterate here
            ++laser;
        }
    }

    // Fire laser only if cooldown has passed
    if (buzzy.fireLaser() && this->fireClock.getElapsedTime() > this->fireCooldown)
    {
        // Create a new laser blast at Buzzy's position
        ECE_LaserBlast playerLaser(playerLaserTexture, buzzy, false, windowSize);
        laserBlasts.push_back(playerLaser);
        this->fireClock.restart();
    }
}


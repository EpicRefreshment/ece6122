/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the source file for the ECE_Buzzy class and implements all functions
and maintains all variables defined in ECE_Buzzy.h.
This class is derived from the SFML Sprite class.
ECE_Buzzy manages details for Buzzy, initiatilizes the object,
maintains position, manages spawn location, moves Buzzy, and detects collisions with other objects
and fires laser.

*/

#include "ECE_Buzzy.h"

using namespace sf;
using namespace std;

/*
This is the constructor for ECE_Buzzy. The constructor initializes
just a few variables. Another member function is called by the game manager
thatfinishes initializing Buzzy

Arguments:
    N/A

Return Values:
    ECE_Buzzy
*/
ECE_Buzzy::ECE_Buzzy()
{
    // Set speed
    buzzySpeed = 300.0f;

    // Set fire cooldown
    fireCooldown = milliseconds(500); // 250 ms cooldown

    // Set buzzy lives
    buzzyLives = 0;
}


/*
This function finishes setting buzzy based on function arguments.
This function sets texture for sprite, stores variables to manage Buzzy, scales Buzzy, 
and sets initial position.

Arguments:
    texture - texture to set for sprite
    windowSize - window size to help scale the object appropriately

Return Values:
    void
*/
void ECE_Buzzy::setupBuzzy(const Texture& texture, Vector2u windowSize)
{
    // Update screen boundary
    screenBoundary = windowSize;

    // Initialize Buzzy with its texture
    this->setTexture(texture);

    // Get current size of buzzy sprite from texture
    Vector2u textureSize = texture.getSize();

    // Scale buzzy to fit screen proportionally and set initial position
    scaleBuzzy(textureSize);

    // Update size and position after scaling
    buzzyBoundary = this->getGlobalBounds();
    buzzySize = buzzyBoundary.getSize();
    buzzyPos = buzzyBoundary.getPosition();

    // Set initial position
    setStartPosition();
}

/*
This function scales Buzzy based on the size of the window to keep things proportional

Arguments:
    textureSize - size of texture

Return Values:
    void
*/
void ECE_Buzzy::scaleBuzzy(Vector2u textureSize)
{
    // Scale buzzy to fit screen proportionally
    float scaleX = (float) screenBoundary.y / textureSize.x; // Maintain aspect ratio to avoid stretching image
    float scaleY = (float) screenBoundary.y / textureSize.y;

    // Rescale sprite
    this->setScale(scaleX / 10.0f, scaleY / 10.0f); // Scale down to 1/10 size
}

/*
This function sets the starting location of Buzzy

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Buzzy::setStartPosition()
{
    // Set initial position
    buzzyPos.x = (screenBoundary.x / 2.0f) - buzzySize.x / 2.0f; // Center horizontally
    buzzyPos.y = buzzySize.y * 0.25f; // Keep Buzzy a bit below top of window
    this->setPosition(buzzyPos.x, buzzyPos.y);
}

/*
This function updates the position of the laser. Calls two helper functions to move Buzzy in
either left or right based on user input (Left and Right on keyboard).

Arguments:
    frameTime - used to scale the distance the object is moved each time based on an assumed frame time

Return Values:
    void
*/
void ECE_Buzzy::update(float frameTime)
{
    // Move Buzzy left
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        moveLeft(frameTime);
    }

    // Move Buzzy right
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        moveRight(frameTime);
    }

    // Update sprite position
    this->setPosition(buzzyPos.x, buzzyPos.y);
}

/*
This function moves Buzzy left.

Arguments:
    frameTime - used to scale the distance the object is moved each time based on an assumed frame time

Return Values:
    void
*/
void ECE_Buzzy::moveLeft(float frameTime)
{
    buzzyPos.x -= buzzySpeed * frameTime;
    if (buzzyPos.x < 0)
    {
        // Prevent moving off left edge
        buzzyPos.x = 0;
    }
}

/*
This function moves buzzy right.

Arguments:
    frameTime - used to scale the distance the object is moved each time based on an assumed frame time

Return Values:
    void
*/
void ECE_Buzzy::moveRight(float frameTime)
{
    buzzyPos.x += buzzySpeed * frameTime; 
    if (buzzyPos.x > screenBoundary.x - buzzySize.x)
    { 
        // Prevent moving off right edge
        buzzyPos.x = screenBoundary.x - buzzySize.x;
    }
}

/*
This function determines if the laser can be fired based on a cooldown
and user input (Spacebar on keyboard). 
This serves to keep Buzzy from firing consecutively too fast.

Arguments:
    N/A

Return Values:
    bool - flag indicating Buzzy can fire the laser
*/
bool ECE_Buzzy::fireLaser()
{
    // Return true if spacebar is pressed and cooldown has passed
    if (Keyboard::isKeyPressed(Keyboard::Space) && fireClock.getElapsedTime() > fireCooldown)
    {
        fireClock.restart(); // restart the cooldown
        return true;
    }
    else
    {
        return false; // laser cannot be fired yet
    }
}

/*
This function detects if there is a collision with another Sprite

Arguments:
    object - Sprite that the object may or may not be colliding with.

Return Values:
    bool - true if collision detected, false if no collision
*/
bool ECE_Buzzy::collisionDetected(const Sprite& object)
{
    // Check if Sprite rectangle boundaries intersect
    return this->getGlobalBounds().intersects(object.getGlobalBounds());
}

/*
This function gets the size of Buzzy

Arguments:
    N/A

Return Values:
    Vector2f - size of Buzzy on x and y axis
*/
Vector2f ECE_Buzzy::getSize()
{
    return buzzySize;
}

/*
This function gets the speed of Buzzy

Arguments:
    N/A

Return Values:
    float - speed of Buzzy
*/
float ECE_Buzzy::getSpeed()
{
    return buzzySpeed;
}

/*
This function gets the boundaries of the enemy Sprite as it exists in the game world
Left most position, top most position, width, and height

Arguments:
    N/A

Return Values:
    FloatRect - global boundaries of enemy
*/
FloatRect ECE_Buzzy::getBoundary()
{
    return buzzyBoundary;
}

/*
This function sets how many lives Buzzy has at start of game. Called by game manager.

Arguments:
    lives - number of lives

Return Values:
    void
*/
void ECE_Buzzy::setLives(int lives)
{
    buzzyLives = lives;
}

/*
This function gets how many lives Buzzy has left.

Arguments:
    N/A

Return Values:
    int - number of lives
*/
int ECE_Buzzy::getLives()
{
    return buzzyLives;
}
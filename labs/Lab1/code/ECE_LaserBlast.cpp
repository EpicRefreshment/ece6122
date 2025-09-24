/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the source file for the ECE_LaserBlast class and implements all functions
and maintains all variables defined in ECE_LaserBlast.h.
This class is derived from the SFML Sprite class.
ECE_LaserBlast manages details for the laser, initiatilizes the object,
maintains position, moves laser, and detects collisions with other objects
and screen boundaries.

*/

#include "ECE_LaserBlast.h"

using namespace sf;
using namespace std;

/*
This is the constructor for ECE_LaserBlast. The constructor initializes
the laser blast based on function arguments. This function sets texture for sprite,
stores variables to manage laser, scales laser, and sets initial position.

Arguments:
    texture - texture to set for sprite
    shooter - object that is firing the laser
    isEnemy - flag that determines if shooter is an enemy or buzzy
    windowSize - window size to help scale the object appropriately

Return Values:
    ECE_LaserBlast
*/
ECE_LaserBlast::ECE_LaserBlast(const Texture& texture, const Sprite& shooter, bool isEnemy, Vector2u windowSize)
{
    // Set screen boundary to window size argument
    screenBoundary = windowSize;

    // Set speed
    laserSpeed = 150.0f;
    
    // Create sprite and scale to fit screen proportionally
    this->setTexture(texture);

    // Save shooter position and size
    shooterPos = shooter.getPosition();
    shooterSize = shooter.getGlobalBounds().getSize();

    // Save isEnemy flag
    this->isEnemy = isEnemy;

    // Get current size of laser sprite from texture
    Vector2u textureSize = texture.getSize();

    // Scale laser to fit screen proportionally and set initial position
    scaleLaser(textureSize);

    // Update size and position after scaling
    laserBoundary = this->getGlobalBounds();
    laserSize = laserBoundary.getSize();
    laserPos = laserBoundary.getPosition();

    // Set initial position
    this->setInitialPosition();
}

/*
This function scales the laser based on the size of the window to keep things proportional

Arguments:
    textureSize - size of texture

Return Values:
    void
*/
void ECE_LaserBlast::scaleLaser(Vector2u textureSize)
{
    // Scale laser to fit screen proportionally
    float scaleX = (float) screenBoundary.x / textureSize.x;
    float scaleY = (float) screenBoundary.y / textureSize.y;

    this->setScale(scaleX / 120.0f, scaleY / 20.0f); // Scale down: width by 1/100, height by 1/20
}

/*
This function sets the starting position for the laser based on the position and size
of the sprite firing the laser

Arguments:
    N/A

Return Values:
    void
*/
void ECE_LaserBlast::setInitialPosition()
{
    // Set initial position based on shooter's position
    laserPos.x = shooterPos.x + (shooterSize.x / 2.0f); // Center laser on shooter horizontally
    if (isEnemy)
    {
        laserPos.y = shooterPos.y; // Start at the top of the enemy shooter
    }
    else
    {
        laserPos.y = shooterPos.y + (shooterSize.y); // Start below the player shooter
    } 
    this->setPosition(laserPos.x, laserPos.y);
}

/*
This function updates the position of the laser. Calls one of two helper functions depending
on if the shooter is an enemy or Buzzy.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_LaserBlast::update(Time lastFrameTime)
{
    if (isEnemy)
    {
        // Move laser blast upwards
        moveUp(lastFrameTime);
    }
    else
    {
        // Move laser blast downwards
        moveDown(lastFrameTime);
    }

    // Update sprite position
    this->setPosition(laserPos.x, laserPos.y);

    // Update laser boundary to reflect new position and size
    laserBoundary = this->getGlobalBounds();
}

/*
This function moves the laser up.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_LaserBlast::moveUp(Time lastFrameTime)
{
    laserPos.y -= (laserSpeed * lastFrameTime.asSeconds());
}

/*
This function moves the laser down.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_LaserBlast::moveDown(Time lastFrameTime)
{
    laserPos.y += (laserSpeed * lastFrameTime.asSeconds());
}

/*
This function detects if there is a collision with another Sprite

Arguments:
    object - Sprite that the object may or may not be colliding with.

Return Values:
    bool - true if collision detected, false if no collision
*/
bool ECE_LaserBlast::collisionDetected(const Sprite& object)
{
    // Check for collision with another sprite using bounding box intersection
    return this->getGlobalBounds().intersects(object.getGlobalBounds());
}

/*
This function detects collisions with the screen edges.

Arguments:
    N/A

Return Values:
    bool - true if screen edge detected, false if not
*/
bool ECE_LaserBlast::boundaryDetected()
{
    // Check if laser blast has moved off the screen
    return (laserPos.y < 0 || laserPos.y > screenBoundary.y);
}

/*
This function gets the size of the laser

Arguments:
    N/A

Return Values:
    Vector2f - size of laser on x and y axis
*/
Vector2f ECE_LaserBlast::getSize()
{
    return laserSize;
}

/*
This function gets the speed of the laser

Arguments:
    N/A

Return Values:
    float - speed of laser
*/
float ECE_LaserBlast::getSpeed()
{
    return laserSpeed;
}

/*
This function gets the boundaries of the laser Sprite as it exists in the game world
Left most position, top most position, width, and height

Arguments:
    N/A

Return Values:
    FloatRect - global boundaries of laser
*/
FloatRect ECE_LaserBlast::getBoundary()
{
    return laserBoundary;
}
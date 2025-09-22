/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the source file for the ECE_Enemy class and implements all functions
and maintains all variables defined in ECE_Enemy.h.
This class is derived from the SFML Sprite class.
ECE_Enemy manages details for the enemy, initiatilizes the object,
maintains position, manages spawn location, moves enemy, and detects collisions with other objects
and fires laser.

*/

#include <iostream>

#include "ECE_Enemy.h"

using namespace sf;
using namespace std;

/*
This is the constructor for ECE_Enemy. The constructor initializes
the enemy based on function arguments. This function sets texture for sprite,
stores variables to manage the enemy, scales enemy, and sets initial position and spawn location.

Arguments:
    texture - texture to set for sprite
    isBoss - flag that determines if the enemy is a boss or not
    windowSize - window size to help scale the object appropriately

Return Values:
    ECE_Enemy
*/
ECE_Enemy::ECE_Enemy(const Texture& texture, bool isBoss, Vector2u windowSize)
{
    // Update screen boundary
    screenBoundary = windowSize;

    // Set speed
    if (!isBoss)
    {
        enemySpeed = 100.0f;
    }
    else
    {
        enemySpeed = 250.0f; // go faster if boss
    }
    direction = false; // false = left, true = right
    // Set fire cooldown
    fireCooldown = milliseconds(1000); // 1 second cooldown

    // Create sprite and scale to fit screen proportionally
    this->setTexture(texture);

    // Get current size of enemy sprite from texture
    Vector2u textureSize = texture.getSize();

    // Scale enemy to fit screen proportionally and set initial position
    scaleEnemy(textureSize);

    // Update size and position after scaling
    enemyBoundary = this->getGlobalBounds();
    enemySize = enemyBoundary.getSize();
    enemyPos = enemyBoundary.getPosition();

    // Set initial position
    setSpawnLocation();
    setInitialPosition();
}

/*
This function scales the enemy based on the size of the window to keep things proportional

Arguments:
    textureSize - size of texture

Return Values:
    void
*/
void ECE_Enemy::scaleEnemy(Vector2u textureSize)
{
    // Scale enemy to fit screen proportionally
    float scaleX = (float) screenBoundary.y / textureSize.x; // Maintain aspect ratio to avoid stretching image
    float scaleY = (float) screenBoundary.y / textureSize.y;

    // Rescale sprite
    this->setScale(scaleX / 10.0f, scaleY / 10.0f); // Scale down to 1/10 size
}

/*
This function sets the spawn location of the enemy

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Enemy::setSpawnLocation()
{
    // Set spawn boundary to bottom left 1/4 of screen
    enemySpawnBoundary.left = screenBoundary.x - enemySize.x * 1.5f;
    enemySpawnBoundary.top = screenBoundary.y - enemySize.y * 1.5f;
    enemySpawnBoundary.width = enemySize.x * 1.5f;
    enemySpawnBoundary.height = enemySize.y * 1.5f; 

    // Set spawn position to middle of spawn boundary
    enemySpawnPos.x = enemySpawnBoundary.left + enemySize.x * 0.25f;
    enemySpawnPos.y = enemySpawnBoundary.top + enemySize.y * 0.25f;
}

/*
This function sets the starting position for the enemy within the spawn boundaries

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Enemy::setInitialPosition()
{
    // Set initial position
    enemyPos.x = enemySpawnPos.x;
    enemyPos.y = enemySpawnPos.y;
    this->setPosition(enemyPos.x, enemyPos.y);
}

/*
This function updates the position of the laser. Calls three helper functions to move the enemy in
various directions: left, right, and up. Moves up if the enemy reaches the edge of the screen.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_Enemy::update(Time lastFrameTime)
{
    // Move enemy to the side
    if (direction) // move right
    {
        moveRight(lastFrameTime);
        if (enemyPos.x > screenBoundary.x - enemySize.x * 1.5f) // Prevent moving off right edge of spawn boundary
        {
            moveUp();
        }
    }
    else // move left
    {
        moveLeft(lastFrameTime);
        if (enemyPos.x < enemySize.x * 0.5f) // Prevent moving off left edge of spawn boundary
        {
            moveUp();
        }
    }

    if (enemyPos.y < screenBoundary.y / 2.0f)
    {
        // increase the speed once it gets closer to the player. Increase the challenge.
        enemySpeed = 200.0f;
    }
    
    this->setPosition(enemyPos.x, enemyPos.y);
}

/*
This function moves the enemy left.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_Enemy::moveLeft(Time lastFrameTime)
{
    enemyPos.x -= (enemySpeed * lastFrameTime.asSeconds());
}

/*
This function moves the enemy right.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_Enemy::moveRight(Time lastFrameTime)
{
    enemyPos.x += (enemySpeed * lastFrameTime.asSeconds());
}

/*
This function moves the enemy up.

Arguments:
    frameTime - used to scale the distance the object is moved each time based on an assumed frame time

Return Values:
    void
*/
void ECE_Enemy::moveUp()
{
    direction = !direction; // flip direction
    // Move up a row
    enemyPos.y -= enemySize.y * 2.0f;
}

/*
This function determines if the laser can be fired based on a cooldown. This serves
to keep the enemy from firing consecutively too fast. The game manager handles logic 
determining if the enemy wants to fire or not.

Arguments:
    N/A

Return Values:
    bool - flag indicating the enemy can fire the laser
*/
bool ECE_Enemy::fireLaser()
{
    // Return true if cooldown has passed to indicate a laser can be fired
    if (fireClock.getElapsedTime() > fireCooldown)
    {
        fireClock.restart(); // this should only restart once laser has already been fired.
        return true;
    }
    else
    {
        return false;
    }
}

/*
This function detects if there is a collision with another Sprite

Arguments:
    object - Sprite that the object may or may not be colliding with.

Return Values:
    bool - true if collision detected, false if no collision
*/
bool ECE_Enemy::collisionDetected(const Sprite& object)
{
    // Check for bounding box intersection
    return this->getGlobalBounds().intersects(object.getGlobalBounds());
}

/*
This function checks if the spawn location is clear so another enemy can spawn.

Arguments:
    object - Sprite that the object may or may not be colliding with.

Return Values:
    bool - true if spawn clear, false if not
*/
bool ECE_Enemy::spawnBoundaryClear()
{
    if (enemyPos.x < enemySpawnBoundary.left - (enemySize.x * 1.25f))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
This function gets the spawn position

Arguments:
    N/A

Return Values:
    Vector2f - spawn X and Y position
*/
Vector2f ECE_Enemy::getSpawnPosition()
{
    return enemySpawnPos;
}

/*
This function gets the size of the enemy

Arguments:
    N/A

Return Values:
    Vector2f - size of enemy on x and y axis
*/
Vector2f ECE_Enemy::getSize()
{
    return enemySize;
}

/*
This function gets the boundaries of the enemy Sprite as it exists in the game world
Left most position, top most position, width, and height

Arguments:
    N/A

Return Values:
    FloatRect - global boundaries of enemy
*/
FloatRect ECE_Enemy::getBoundary()
{
    return enemyBoundary;
}

/*
This function gets the speed of the enemy

Arguments:
    N/A

Return Values:
    float - speed of enemy
*/
float ECE_Enemy::getSpeed()
{
    return enemySpeed;
}
#include <iostream>

#include "ECE_Enemy.h"

using namespace sf;
using namespace std;

ECE_Enemy::ECE_Enemy(const Texture& texture, Vector2u windowSize)
{
    // Update screen boundary
    screenBoundary = windowSize;

    // Set speed
    enemySpeed = 150.0f;
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

void ECE_Enemy::scaleEnemy(Vector2u textureSize)
{
    // Scale enemy to fit screen proportionally
    float scaleX = (float) screenBoundary.y / textureSize.x; // Maintain aspect ratio to avoid stretching image
    float scaleY = (float) screenBoundary.y / textureSize.y;

    // Rescale sprite
    this->setScale(scaleX / 10.0f, scaleY / 10.0f); // Scale down to 1/10 size
}

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

void ECE_Enemy::setInitialPosition()
{
    // Set initial position
    enemyPos.x = enemySpawnPos.x;
    enemyPos.y = enemySpawnPos.y;
    this->setPosition(enemyPos.x, enemyPos.y);
}

void ECE_Enemy::update()
{
    // Move enemy to the side
    if (direction) // move right
    {
        moveRight();
        if (enemyPos.x > screenBoundary.x - enemySize.x * 1.5f) // Prevent moving off right edge of spawn boundary
        {
            moveUp();
        }
    }
    else // move left
    {
        moveLeft();
        if (enemyPos.x < enemySize.x * 0.5f) // Prevent moving off left edge of spawn boundary
        {
            moveUp();
        }
    }

    if (enemyPos.y < screenBoundary.y / 2.0f)
    {
        enemySpeed = 200.0f;
    }
    
    this->setPosition(enemyPos.x, enemyPos.y);
}

void ECE_Enemy::moveLeft()
{
    enemyPos.x -= enemySpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
}

void ECE_Enemy::moveRight()
{
    enemyPos.x += enemySpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
}

void ECE_Enemy::moveUp()
{
    direction = !direction; // flip direction
    // Move up a row
    enemyPos.y -= enemySize.y * 2.0f;
}

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

bool ECE_Enemy::collisionDetected(const Sprite& object)
{
    // Check for bounding box intersection
    return this->getGlobalBounds().intersects(object.getGlobalBounds());
}

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

Vector2f ECE_Enemy::getSpawnPosition()
{
    return enemySpawnPos;
}

Vector2f ECE_Enemy::getSize()
{
    return enemySize;
}

Vector2f ECE_Enemy::getPosition()
{
    return enemyPos;
}

FloatRect ECE_Enemy::getBoundary()
{
    return enemyBoundary;
}

float ECE_Enemy::getSpeed()
{
    return enemySpeed;
}
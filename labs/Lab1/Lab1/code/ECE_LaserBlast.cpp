#include <iostream>

#include "ECE_LaserBlast.h"

using namespace sf;
using namespace std;

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

void ECE_LaserBlast::scaleLaser(Vector2u textureSize)
{
    // Scale laser to fit screen proportionally
    float scaleX = (float) screenBoundary.x / textureSize.x;
    float scaleY = (float) screenBoundary.y / textureSize.y;
    float uniformScale = std::min(scaleX / 100.0f, scaleY / 20.0f); // Use the smaller scale to maintain aspect ratio

    this->setScale(scaleX / 100.0f, scaleY / 20.0f); // Scale down: width by 1/100, height by 1/20
    this->setScale(uniformScale, uniformScale);
}

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

void ECE_LaserBlast::update(float frameTime)
{
    if (isEnemy)
    {
        // Move laser blast upwards
        moveUp(frameTime);
    }
    else
    {
        // Move laser blast downwards
        moveDown(frameTime);
    }

    // Update sprite position
    this->setPosition(laserPos.x, laserPos.y);

    // Update laser boundary to reflect new position and size
    laserBoundary = this->getGlobalBounds();
}

void ECE_LaserBlast::moveUp(float frameTime)
{
    laserPos.y -= laserSpeed * frameTime;
}

void ECE_LaserBlast::moveDown(float frameTime)
{
    laserPos.y += laserSpeed * frameTime;
}

bool ECE_LaserBlast::collisionDetected(const Sprite& object)
{
    // Check for collision with another sprite using bounding box intersection
    return this->getGlobalBounds().intersects(object.getGlobalBounds());
}

bool ECE_LaserBlast::boundaryDetected()
{
    // Check if laser blast has moved off the screen
    return (laserPos.y < 0 || laserPos.y > screenBoundary.y);
}

Vector2f ECE_LaserBlast::getSize()
{
    return laserSize;
}

float ECE_LaserBlast::getSpeed()
{
    return laserSpeed;
}

FloatRect ECE_LaserBlast::getBoundary()
{
    return laserBoundary;
}
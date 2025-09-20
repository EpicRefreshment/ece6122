#include "ECE_LaserBlast.h"

using namespace sf;
using namespace std;

ECE_LaserBlast::ECE_LaserBlast(const Texture& texture, const Sprite& shooter, bool isEnemy, Vector2u windowSize)
{
    // Set screen boundary to window size argument
    screenBoundary = windowSize;
    
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
    this->scaleLaser(textureSize);

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
    float scaleX = (float) screenBoundary.x / laserSize.x; // Maintain aspect ratio to avoid stretching image
    float scaleY = (float) screenBoundary.y / laserSize.y;

    // Rescale sprite
    this->setScale(scaleX / 60.0f, scaleY / 60.0f); // Scale down to 1/10 size
}

void ECE_LaserBlast::setInitialPosition()
{
    // Set initial position based on shooter's position
    laserPos.x = shooterPos.x + (shooterSize.x /*/ 9.0f*/); // Center laser on shooter horizontally
    if (isEnemy)
    {
        laserPos.y = shooterPos.y; // Start at the top of the enemy shooter
        laserSpeed = 200.0f; // Move upwards
    }
    else
    {
        laserPos.y = shooterPos.y + (shooterSize.y /*/ 5.0f*/); // Start below the player shooter
        laserSpeed = -200.0f; // Move downwards
    } 
    this->setPosition(laserPos.x, laserPos.y);
}

void ECE_LaserBlast::update()
{
    if (isEnemy)
    {
        // Move laser blast upwards
        laserPos.y += laserSpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
    }
    else
    {
        // Move laser blast downwards
        laserPos.x -= laserSpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
    }

    // Update sprite position
    this->setPosition(laserPos.x, laserPos.y);
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

Vector2f ECE_LaserBlast::getPosition()
{
    return laserPos;
}

float ECE_LaserBlast::getSpeed()
{
    return laserSpeed;
}

FloatRect ECE_LaserBlast::getBoundary()
{
    return laserBoundary;
}
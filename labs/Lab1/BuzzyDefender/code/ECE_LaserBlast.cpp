#include "ECE_LaserBlast.h"

using namespace sf;
using namespace std;

ECE_LaserBlast::ECE_LaserBlast(const Texture& texture, const Sprite& shooter, bool isEnemy, Vector2u windowSize)
{
    // Create sprite and scale to fit screen proportionally
    this->setTexture(texture);
    laserSize = texture.getSize();

    // Save shooter position and size
    shooterPos = shooter.getPosition();
    shooterSize = shooter.getTexture()->getSize();

    // Save isEnemy flag
    this->isEnemy = isEnemy;

    // Set screen boundary to window size argument
    screenBoundary = windowSize;

    this->scaleLaser();
    this->setInitialPosition();
}

void ECE_LaserBlast::scaleLaser()
{
    // Scale buzzy to fit screen proportionally
    float scaleX = (float) screenBoundary.y / laserSize.x; // Maintain aspect ratio to avoid stretching image
    float scaleY = (float) screenBoundary.y / laserSize.y;

    // Rescale sprite
    this->setScale(scaleX / 60.0f, scaleY / 60.0f); // Scale down to 1/10 size
}

void ECE_LaserBlast::setInitialPosition()
{
    // Set initial position based on shooter's position
    laserPosX = shooterPos.x + (shooterSize.x / 9.0f); // Center laser on shooter horizontally
    if (isEnemy)
    {
        laserPosY = shooterPos.y; // Start at the top of the enemy shooter
        laserSpeed = 200.0f; // Move upwards
    }
    else
    {
        laserPosY = shooterPos.y + (shooterSize.y / 5.0f); // Start below the player shooter
        laserSpeed = -200.0f; // Move downwards
    } 
    this->setPosition(laserPosX, laserPosY);
}

void ECE_LaserBlast::update()
{
    if (isEnemy)
    {
        // Move laser blast upwards
        laserPosY += laserSpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
    }
    else
    {
        // Move laser blast downwards
        laserPosY -= laserSpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
    }

    // Update sprite position
    this->setPosition(laserPosX, laserPosY);
}

bool ECE_LaserBlast::collisionDetected(const Sprite& object)
{
    // Check for collision with another sprite using bounding box intersection
    return this->getGlobalBounds().intersects(object.getGlobalBounds());
}

bool ECE_LaserBlast::boundaryDetected()
{
    // Check if laser blast has moved off the screen
    return (laserPosY < 0 || laserPosY > screenBoundary.y);
}
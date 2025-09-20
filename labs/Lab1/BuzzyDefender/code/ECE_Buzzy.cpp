#include "ECE_Buzzy.h"

using namespace sf;
using namespace std;

ECE_Buzzy::ECE_Buzzy()
{
    // Set speed
    buzzySpeed = 300.0f;

    // Set fire cooldown
    fireCooldown = milliseconds(250); // 250 ms cooldown
}

void ECE_Buzzy::setupBuzzy(const Texture& texture, Vector2u windowSize)
{
    // Update screen boundary
    screenBoundary = windowSize;

    // Initialize Buzzy with its texture
    this->setTexture(texture);

    // Get current size of buzzy sprite from texture
    Vector2u textureSize = texture.getSize();

    // Scale buzzy to fit screen proportionally and set initial position
    this->scaleBuzzy(textureSize);

    // Update size and position after scaling
    buzzyBoundary = this->getGlobalBounds();
    buzzySize = buzzyBoundary.getSize();
    buzzyPos = buzzyBoundary.getPosition();

    // Set initial position
    this->setInitialPosition();
}

void ECE_Buzzy::scaleBuzzy(Vector2u textureSize)
{
    // Scale buzzy to fit screen proportionally
    float scaleX = (float) screenBoundary.y / textureSize.x; // Maintain aspect ratio to avoid stretching image
    float scaleY = (float) screenBoundary.y / textureSize.y;

    // Rescale sprite
    this->setScale(scaleX / 10.0f, scaleY / 10.0f); // Scale down to 1/10 size
}

void ECE_Buzzy::setInitialPosition()
{
    // Set initial position
    //buzzyPos.x = (screenBoundary.x / 2.0f) - buzzySize.x / 20.0f; // Center horizontally
    buzzyPos.x = (screenBoundary.x / 2.0f) - buzzySize.x; // Center horizontally
    //buzzyPos.y = buzzySize.y / 20.0f; // Keep Buzzy a bit below top of window
    buzzyPos.y = buzzySize.y * 0.5f; // Keep Buzzy a bit below top of window
    this->setPosition(buzzyPos.x, buzzyPos.y);
}

void ECE_Buzzy::update()
{
    // Move Buzzy left
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        buzzyPos.x -= buzzySpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
        if (buzzyPos.x < 0)
        {
            // Prevent moving off left edge
            buzzyPos.x = 0;
        }
    }

    // Move Buzzy right
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        buzzyPos.x += buzzySpeed * 0.016f;
        if (buzzyPos.x > screenBoundary.x - buzzySize.x /*/ 4.0f*/)
        { 
            // Prevent moving off right edge
            buzzyPos.x = screenBoundary.x - buzzySize.x /*/ 4.0f*/;
        }
    }

    // Update sprite position
    this->setPosition(buzzyPos.x, buzzyPos.y);
}

bool ECE_Buzzy::fireLaser()
{
    // Return true if spacebar is pressed and cooldown has passed
    if (Keyboard::isKeyPressed(Keyboard::Space) && fireClock.getElapsedTime() > fireCooldown)
    {
        fireClock.restart();
        return true;
    }
    else
    {
        return false;
    }
}

bool ECE_Buzzy::collisionDetected(const Sprite& object)
{
    // Check for bounding box intersection
    return this->getGlobalBounds().intersects(object.getGlobalBounds());
}

Vector2f ECE_Buzzy::getSize()
{
    return buzzySize;
}

Vector2f ECE_Buzzy::getPosition()
{
    return buzzyPos;
}

float ECE_Buzzy::getSpeed()
{
    return buzzySpeed;
}

FloatRect ECE_Buzzy::getBoundary()
{
    return buzzyBoundary;
}
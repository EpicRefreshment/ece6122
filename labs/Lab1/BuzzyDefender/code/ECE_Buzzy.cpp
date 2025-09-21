#include "ECE_Buzzy.h"

using namespace sf;
using namespace std;

ECE_Buzzy::ECE_Buzzy()
{
    // Set speed
    buzzySpeed = 300.0f;

    // Set fire cooldown
    fireCooldown = milliseconds(500); // 250 ms cooldown

    // Set buzzy lives
    buzzyLives = 0;
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
    scaleBuzzy(textureSize);

    // Update size and position after scaling
    buzzyBoundary = this->getGlobalBounds();
    buzzySize = buzzyBoundary.getSize();
    buzzyPos = buzzyBoundary.getPosition();

    // Set initial position
    setStartPosition();
}

void ECE_Buzzy::scaleBuzzy(Vector2u textureSize)
{
    // Scale buzzy to fit screen proportionally
    float scaleX = (float) screenBoundary.y / textureSize.x; // Maintain aspect ratio to avoid stretching image
    float scaleY = (float) screenBoundary.y / textureSize.y;

    // Rescale sprite
    this->setScale(scaleX / 10.0f, scaleY / 10.0f); // Scale down to 1/10 size
}

void ECE_Buzzy::setStartPosition()
{
    // Set initial position
    buzzyPos.x = (screenBoundary.x / 2.0f) - buzzySize.x / 2.0f; // Center horizontally
    buzzyPos.y = buzzySize.y * 0.25f; // Keep Buzzy a bit below top of window
    this->setPosition(buzzyPos.x, buzzyPos.y);
}

void ECE_Buzzy::update()
{
    // Move Buzzy left
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        moveLeft();
    }

    // Move Buzzy right
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        moveRight();
    }

    // Update sprite position
    this->setPosition(buzzyPos.x, buzzyPos.y);
}

void ECE_Buzzy::moveLeft()
{
    buzzyPos.x -= buzzySpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
    if (buzzyPos.x < 0)
    {
        // Prevent moving off left edge
        buzzyPos.x = 0;
    }
}

void ECE_Buzzy::moveRight()
{
    buzzyPos.x += buzzySpeed * 0.016f; // Assuming 60 FPS, so frame time ~0.016s
    if (buzzyPos.x > screenBoundary.x - buzzySize.x /*/ 4.0f*/)
    { 
        // Prevent moving off right edge
        buzzyPos.x = screenBoundary.x - buzzySize.x /*/ 4.0f*/;
    }
}

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

bool ECE_Buzzy::collisionDetected(const Sprite& object)
{
    // Check if Sprite rectangle boundaries intersect
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

void ECE_Buzzy::setLives(int lives)
{
    buzzyLives = lives;
}

int ECE_Buzzy::getLives()
{
    return buzzyLives;
}
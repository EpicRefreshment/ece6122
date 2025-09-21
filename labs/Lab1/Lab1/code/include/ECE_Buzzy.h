#pragma once

// Include SFML libraries here
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_Buzzy: public Sprite
{
public:
    // Public member functions
    ECE_Buzzy();
    void setupBuzzy(const Texture& texture, Vector2u windowSize);
    void setStartPosition();
    void update(float frameTime);
    void moveRight(float frameTime);
    void moveLeft(float frameTime);
    bool fireLaser();
    bool collisionDetected(const Sprite& object);
    Vector2f getSize();
    Vector2f getPosition();
    float getSpeed();
    FloatRect getBoundary();
    void setLives(int lives);
    int getLives();
private:
    // Private member functions
    void scaleBuzzy(Vector2u textureSize);

    // Private member variables
    Vector2f buzzySize;
    Vector2f buzzyPos;
    float buzzySpeed;
    FloatRect buzzyBoundary;
    Vector2u screenBoundary;
    Clock fireClock;
    Time fireCooldown;
    int buzzyLives;
};
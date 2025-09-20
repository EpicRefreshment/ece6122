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
    void scaleBuzzy(Vector2u textureSize);
    void setInitialPosition();
    void update();
    bool fireLaser();
    bool collisionDetected(const Sprite& object);
    Vector2f getSize();
    Vector2f getPosition();
    float getSpeed();
    FloatRect getBoundary();

    // Public member variables
private:
    // Private member variables
    Vector2f buzzySize;
    Vector2f buzzyPos;
    float buzzySpeed;
    FloatRect buzzyBoundary;
    Vector2u screenBoundary;
};
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
    void scaleBuzzy();
    void setInitialPosition();
    void update();
    bool fireLaser();
    bool collisionDetected(const Sprite& object);

    // Public member variables
    Vector2u buzzySize;
    float buzzyPosX;
    float buzzyPosY;
private:
    // Private member variables
    float buzzySpeed;
    Vector2u screenBoundary;
};
#ifndef ECE_BUZZY_H
#define ECE_BUZZY_H

// Include SFML libraries here
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_Buzzy: public Sprite
{
public:
    // Public member functions
    ECE_Buzzy();
    void scaleBuzzy(Vector2u windowSize);
    void setInitialPosition();
    void update();
    void fireLaser();
    bool collisionDetected(const Sprite& object);
private:
    // Private member variables
    Texture buzzyTexture;
    Vector2u buzzySize;
    float buzzySpeed;
    float buzzyPosX;
    float buzzyPosY;
    Vector2u screenBoundary;
};

#endif // ECE_BUZZY_H
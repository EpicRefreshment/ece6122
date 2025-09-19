#ifndef ECE_BUZZY_H
#define ECE_BUZZY_H

// Include SFML libraries here
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_Buzzy: public Sprite
{
public:
    ECE_Buzzy(Vector2u windowSize);
    void update();
    bool collisionDetected(const Sprite& object);
    Sprite spriteBuzzy;
private:
    Texture textureBuzzy;
    Vector2u buzzySize;
    float speedBuzzy;
    float positionXBuzzy;
    float positionYBuzzy;
    Vector2u screenBoundary;
};

#endif // ECE_BUZZY_H
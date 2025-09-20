#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_LaserBlast : public Sprite
{
public:
    // Public member functions
    ECE_LaserBlast(const Texture& texture, const Sprite& shooter, bool isEnemy, Vector2u windowSize);
    void update();
    bool collisionDetected(const Sprite& object);
    bool boundaryDetected();
    Vector2f getSize();
    Vector2f getPosition();
    float getSpeed();
    FloatRect getBoundary();

private:
    // Private member functions
    void scaleLaser(Vector2u textureSize);
    void setInitialPosition();
    
    // Private member variables
    Vector2f laserSize;
    Vector2f laserPos;
    Vector2f shooterPos;
    Vector2f shooterSize;
    bool isEnemy;
    float laserSpeed;
    FloatRect laserBoundary;
    Vector2u screenBoundary;
};
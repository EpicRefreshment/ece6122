#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_LaserBlast : public Sprite
{
public:
    ECE_LaserBlast(const Texture& texture, const Sprite& shooter, bool isEnemy, Vector2u windowSize);
    void update();
    bool collisionDetected(const Sprite& object);
    bool boundaryDetected();

private:
    void scaleLaser();
    void setInitialPosition();
    Vector2u laserSize;
    Vector2f shooterPos;
    Vector2u shooterSize;
    bool isEnemy;
    float laserSpeed;
    float laserPosX;
    float laserPosY;
    Vector2u screenBoundary;
};
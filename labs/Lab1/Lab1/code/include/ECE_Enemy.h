#pragma once

// Include SFML libraries here
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_Enemy: public Sprite
{
public:
    // Public member functions
    ECE_Enemy(const Texture& texture, bool isBoss, Vector2u windowSize);
    void update(float frameTime);
    void moveLeft(float frameTime);
    void moveRight(float frameTime);
    void moveUp();
    bool fireLaser();
    bool collisionDetected(const Sprite& object);
    bool spawnBoundaryClear();
    Vector2f getSpawnPosition();
    Vector2f getSize();
    Vector2f getPosition();
    FloatRect getBoundary();
    float getSpeed();
private:
    // Private member functions
    void scaleEnemy(Vector2u textureSize);
    void setSpawnLocation();
    void setInitialPosition();

    // Private member variables
    float enemySpeed;
    Vector2f enemySize;
    Vector2f enemyPos;
    Vector2f enemySpawnPos;
    FloatRect enemySpawnBoundary;
    FloatRect enemyBoundary;
    Vector2u screenBoundary;
    Clock fireClock;
    Time fireCooldown;
    bool direction; // false = left, true = right
};
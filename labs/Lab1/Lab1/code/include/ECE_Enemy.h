/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the header file for the ECE_Enemy class.
This class is derived from the SFML Sprite class.
ECE_Enemy manages details for the enemy, initiatilizes the object,
maintains position, manages spawn location, moves enemy, and detects collisions with other objects
and fires laser.

*/

#pragma once

// Include SFML libraries here
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_Enemy: public Sprite
{
public:
    /***************************
      Public member functions
    ****************************/

    // Constructor
    ECE_Enemy(const Texture& texture, bool isBoss, Vector2u windowSize);

    // update position
    void update(Time lastFrameTime);

    // functions to check if laser can be fired, enemy collided object, and if enemy can spawn
    bool fireLaser();
    bool collisionDetected(const Sprite& object);
    bool spawnBoundaryClear();
    bool inTopRow();

    // Getters for some enemy private member variables
    Vector2f getSpawnPosition();
    Vector2f getSize();
    FloatRect getBoundary();
    float getSpeed();
private:
    /***************************
      Private member functions
    ****************************/

    // Helper functions to initialize enemy
    void scaleEnemy(Vector2u textureSize); // scale to fit window
    void setSpawnLocation(); // set location for enemies to appear from
    void setInitialPosition();

    // move enemy
    void moveLeft(Time lastFrameTime);
    void moveRight(Time lastFrameTime);
    void moveUp();

    /***************************
      Private member variables
    ****************************/

    // enemy variables 
    float enemySpeed;
    Vector2f enemySize;
    Vector2f enemyPos;

    // enemy spawn variables
    Vector2f enemySpawnPos;
    FloatRect enemySpawnBoundary;
    FloatRect enemyBoundary;

    // window size
    Vector2u screenBoundary;

    // variables to keep enemies from firing consecutively too fast
    Clock fireClock;
    Time fireCooldown;

    // enemy travel direction
    bool direction; // false = left, true = right
};
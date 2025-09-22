/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the header file for the ECE_Buzzy class.
This class is derived from the SFML Sprite class.
ECE_Buzzy manages details for Buzzy, initiatilizes the object,
maintains position, manages spawn location, moves Buzzy, and detects collisions with other objects
and fires laser.

*/

#pragma once

// Include SFML libraries here
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_Buzzy: public Sprite
{
public:
    /***************************
      Public member functions
    ****************************/

    // Constructor
    ECE_Buzzy();

    // initialize Buzzy
    void setupBuzzy(const Texture& texture, Vector2u windowSize);
    void setStartPosition();

    // update position
    void update(Time lastFrameTime);

    // functions to check if laser can be fired or if a collision was detected
    bool fireLaser();
    bool collisionDetected(const Sprite& object);

    // Getters for private member variables
    Vector2f getSize();
    float getSpeed();
    FloatRect getBoundary();
    int getLives();

    // Setters for private member variables
    void setLives(int lives);
private:
    /***************************
      Private member functions
    ****************************/

    // scale buzzy proportionally with game window
    void scaleBuzzy(Vector2u textureSize);

    // Move buzzy
    void moveRight(Time lastFrameTime);
    void moveLeft(Time lastFrameTime);

    /***************************
      Private member variables
    ****************************/

    // buzzy variables
    Vector2f buzzySize;
    Vector2f buzzyPos;
    float buzzySpeed;
    FloatRect buzzyBoundary;

    // window size
    Vector2u screenBoundary;

    // variables to keep enemies from firing consecutively too fast
    Clock fireClock;
    Time fireCooldown;

    // number of lives buzzy has left before game over
    int buzzyLives;
};
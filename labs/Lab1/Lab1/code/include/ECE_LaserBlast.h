/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the header file for the ECE_LaserBlast class.
This class is derived from the SFML Sprite class.
ECE_LaserBlast manages details for the laser, initiatilizes the object,
maintains position, moves laser, and detects collisions with other objects
and screen boundaries.

*/

#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ECE_LaserBlast : public Sprite
{
public:
    /***************************
      Public member functions
    ****************************/

    // Constructor
    ECE_LaserBlast(const Texture& texture, const Sprite& shooter, bool isEnemy, Vector2u windowSize);

    // update position
    void update(Time lastFrameTime);

    // Functions to detect collision with game window or other sprite
    bool collisionDetected(const Sprite& object);
    bool boundaryDetected();

    // Functions to get private member variables
    Vector2f getSize();
    float getSpeed();
    FloatRect getBoundary();

private:
    /***************************
      Private member functions
    ****************************/

    // Helper functions for initializing laser
    void scaleLaser(Vector2u textureSize); // scale laser to fit window size
    void setInitialPosition(); // put the laser in it's starting position

    // Functions for laser movement
    void moveUp(Time lastFrameTime);
    void moveDown(Time lastFrameTime);
    
    /***************************
      Private member variables
    ****************************/

    // window size
    Vector2u screenBoundary; 

    // laser variables
    Vector2f laserSize;
    Vector2f laserPos; 
    float laserSpeed;
    FloatRect laserBoundary; // stores sprite bounds

    // variables for object firing the laser
    Vector2f shooterSize;
    Vector2f shooterPos;

    // flag set to true if an enemy shoots the laser. False if buzzy.
    bool isEnemy; 
};
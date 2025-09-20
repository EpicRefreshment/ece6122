#pragma once

#include <list>

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "ECE_Buzzy.h"
#include "ECE_Enemy.h"
#include "ECE_LaserBlast.h"

using namespace sf;
using namespace std;

class ECE_Defender: public RenderWindow
{
public:
    // Public member functions
    ECE_Defender();
    void refreshDisplay();
    void updateScene();
    Vector2u getWindowSize();
private:
    // Private member functions
    void loadTextures();
    void setupBackground();
    void spawnEnemy();
    void firePlayerLaser();
    void fireEnemyLasers();
    void updateLasers();
    void updateEnemies();
    void handleCollisions();

    // Private member variables
    VideoMode vm;
    Vector2u windowSize;
    Texture backgroundTexture;
    Texture buzzyTexture;
    Texture enemyTexture;
    Texture playerLaserTexture;
    Texture enemyLaserTexture;
    Sprite backgroundSprite;
    Vector2u backgroundSize;
    ECE_Buzzy buzzy;
    list<ECE_LaserBlast> laserBlasts;
    list<ECE_Enemy> enemies;
    int level;
    int score;
    int lives;
    int maxEnemies;
    int totalEnemies;
    bool gameOver;
};
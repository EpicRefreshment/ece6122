/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the header file for the ECE_Defender class.
This class is derived from the SFML Window class.
ECE_Defender manages all game objects, state, and logic. 

*/

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

    ECE_Defender(); // Constructor
    void refreshDisplay(); // Clears and redraws all appropriate game objects
    void updateScene(); // main function for updating all game objects

    // Functions to manage game state
    void startGame(); // sets flag to start the game
    void pauseGame(); // sets flag to pause the game i.e. show start screen
    bool isGameOver(); // Returns true if game is over
    bool isGameWon(); // Returns true if the player beat the game
    bool isGamePaused(); // Returns true if game is paused

    Vector2u getWindowSize(); // Returns size of game window
private:
    // Private member functions

    // Helper functions to setup the game
    void loadTextures();
    void setupBackgroundSprite();
    void setupStartScreenSprite();
    void setupGameOverSprite();
    void setupGameWonSprite();

    // helper function for game display
    void drawGameObjects();

    // Update game objects
    void updateBuzzy();
    void updateEnemies();
    void updateLasers();
    void updatePlayerLasers();
    void updateEnemyLasers();

    // Handle collisions
    void handleCollisions();
    void handlePlayerCollisions();
    void handleEnemyCollisions();
    void handleLaserCollisions();

    // Helper functions for game objects
    void spawnEnemy();
    void firePlayerLaser();
    void fireEnemyLasers();
    Texture randomEnemyTexture();

    // Private member variables

    // Variables for main game window
    VideoMode vm;
    Vector2u windowSize;

    // Full screen textures and sizes
    Texture backgroundTexture;
    Texture startScreenTexture;
    Texture gameOverScreenTexture;
    Texture gameWonScreenTexture;
    Vector2u backgroundSize;
    Vector2u startScreenSize;
    Vector2u gameOverScreenSize;
    Vector2u gameWonScreenSize;
    Sprite backgroundSprite;
    Sprite startScreenSprite;
    Sprite gameOverScreenSprite;
    Sprite gameWonScreenSprite;

    // Textures for all game objects
    Texture buzzyTexture;
    Texture enemyTexture1;
    Texture enemyTexture2;
    Texture enemyTexture3;
    Texture playerLaserTexture;
    Texture enemyLaserTexture;

    // Buzzy object
    ECE_Buzzy buzzy;

    // List containers for lasers and enemies
    list<ECE_LaserBlast> playerLaserBlasts;
    list<ECE_LaserBlast> enemyLaserBlasts;
    list<ECE_Enemy> enemies;

    // Game logic variables
    int level;
    int score;
    int maxEnemies;
    int totalEnemies;
    float frameTime;
    bool enemyHit;

    // Game state flags
    bool gameWon;
    bool gameOver;
    bool gamePaused;
    bool gameClockStarted;
    bool bossReady;

    // These two are just for keeping the Game Over screen up for a bit
    // before reverting to the start screen
    Clock gameClock;
    Clock bossClock;
    Time gameEndCooldown;
    Time bossCooldown;
};
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
    /***************************
      Public member functions
    ****************************/

    // Constructor
    ECE_Defender();

    // Manage window and objects
    void refreshDisplay(); // Clears and redraws all appropriate game objects
    void updateScene(Time lastFrameTime); // main function for updating all game objects

    // Functions to manage game state
    void startGame(); // sets flag to start the game
    void pauseGame(); // sets flag to pause the game i.e. show start screen
    bool isGameOver(); // Returns true if game is over
    bool isGameWon(); // Returns true if the player beat the game
    bool isGamePaused(); // Returns true if game is paused

    Vector2u getWindowSize(); // Returns size of game window
private:
    /***************************
      Private member functions
    ****************************/

    // Helper functions to setup the game
    void loadTextures(); // load all necessary textures

    // setup sprites that display over the full screen
    void setupBackgroundSprite(); 
    void setupStartScreenSprite();
    void setupGameOverSprite();
    void setupGameWonSprite();

    // helper function for game display
    void drawGameObjects();

    // Update game object positions scaled with time from last frame update
    void updateBuzzy(Time lastFrameTime);
    void updateEnemies(Time lastFrameTime);
    void updateLasers(Time lastFrameTime);
    void updatePlayerLasers(Time lastFrameTime);
    void updateEnemyLasers(Time lastFrameTime);

    // Handle collisions
    void handleCollisions(); // Master function that collects all helper function calls
    void handlePlayerCollisions(); // Handle player collisions with enemy
    void handleEnemyCollisions(); // Handle enemy collisions with lasers
    void handleLaserCollisions(); // Handle enemy laser collisions with player

    // Helper functions for game objects
    void spawnEnemy(); // spawns enemies and adds them to list
    void firePlayerLaser(); // Handles player firing laser
    void fireEnemyLasers(); // Handles enemy firing laser

    /***************************
      Private member variables
    ****************************/

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

    // Full screen sprites
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
    list<ECE_Enemy> topRowEnemies;

    // Game logic variables
    int level; // curent level. Always level 1
    int score; // number of enemies defeated
    int maxEnemies; // max number of enemies in level
    int totalEnemies; // total enemies that have been spawned

    // flag that is true if enemy was hit by a laser from the player, false if not
    bool enemyHit;

    // Game state flags
    bool gameWon; // you won the game!
    bool gameOver; // you lost the game!
    bool gamePaused; // game is paused and showing start screen
    bool gameClockStarted; // true if the game clock has already been started
    bool bossReady; // true if all other enemies have been defeated

    // These two are just for keeping the Game Over screen up for a bit
    // before reverting to the start screen
    Clock gameClock;
    Time gameEndCooldown;

    // keeps the boss on cooldown so it doesn't pop out immediately to create tension
    Clock bossClock;
    Time bossCooldown;
};
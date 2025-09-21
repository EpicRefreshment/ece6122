#include <iostream>

#include "ECE_Defender.h"

using namespace sf;
using namespace std;

ECE_Defender::ECE_Defender()
{
    vm = VideoMode(1920, 1080); // Set window size
    this->create(vm, "Buzzy Defender", Style::Default); // Create the window

    windowSize = this->getSize(); // Get the actual window size

    gamePaused = true;
    gameOver = false;
    gameWon = false;
    gameClockStarted = false;
    bossReady = false;

    frameTime = 0.008f; // Sets speed scaling for all game objects i.e. 8 ms

    // show post game screens for 2 seconds before reverting to start screen
    gameEndCooldown = milliseconds(2000);
    // wait 3 seconds to spawn boss enemy
    bossCooldown = milliseconds(3000);

    loadTextures(); // Load all textures
    setupBackgroundSprite(); // Setup the background
    setupStartScreenSprite(); // Setup the start screen
    setupGameOverSprite(); // Setup the game over screen
    setupGameWonSprite(); // Setup the game won screen

    // Initialize player
    buzzy.setupBuzzy(buzzyTexture, windowSize);
}

void ECE_Defender::refreshDisplay()
{
    // clear the window
    this->clear();

    if (isGamePaused())
    {
        this->draw(startScreenSprite);
    }
    else if (isGameOver())
    {
        this->draw(gameOverScreenSprite);
    }
    else if (isGameWon())
    {
        this->draw(gameWonScreenSprite);
    }
    else
    {
        drawGameObjects();
    }

    // Display updated frame
    this->display();
}

void ECE_Defender::updateScene()
{
    updateBuzzy(); // Update buzzy
    updateEnemies(); // Update all enemies
    updateLasers(); // Update all laser blasts
    handleCollisions(); // handle all collisions
}

/*
------------------------------------------------------------------------------------------------
Functions to handle game state
------------------------------------------------------------------------------------------------
*/

void ECE_Defender::startGame()
{
    // Reset game variables
    level = 1;
    score = 0;
    buzzy.setLives(3);
    maxEnemies = 21;
    totalEnemies = 0;
    enemies.clear();
    playerLaserBlasts.clear();
    enemyLaserBlasts.clear();
    gamePaused = false;
    gameOver = false;
    gameWon = false;
    gameClockStarted = false;
    
    // Reset Buzzy's position
    buzzy.setStartPosition();
}

void ECE_Defender::pauseGame()
{
    while (!gamePaused)
    {
        if (!gameClockStarted)
        {
            gameClock.restart();
            gameClockStarted = true;
        }
        if (gameClock.getElapsedTime() > gameEndCooldown)
        {
            gameOver = false;
            gameWon = false;
            gamePaused = true;
            gameClockStarted = false;
        }
    }
}

bool ECE_Defender::isGameOver()
{
    if (buzzy.getLives() <= 0)
    {
        gameOver = true;
    }
    else
    {
        gameOver = false;
    }
    return gameOver;
}

bool ECE_Defender::isGameWon()
{
    if (totalEnemies >= 21 && enemies.empty())
    {
        gameWon = true;
    }
    else
    {
        gameWon = false;
    }
    return gameWon;
}

bool ECE_Defender::isGamePaused()
{
    return gamePaused;
}

Vector2u ECE_Defender::getWindowSize()
{
    return windowSize;
}

/*
------------------------------------------------------------------------------------------------
Functions to handle game state
------------------------------------------------------------------------------------------------
*/

void ECE_Defender::loadTextures()
{
    // Load all textures from file
    backgroundTexture.loadFromFile("graphics/background.png");
    startScreenTexture.loadFromFile("graphics/Start_Screen.png");
    gameOverScreenTexture.loadFromFile("graphics/gameover.png");
    gameWonScreenTexture.loadFromFile("graphics/success.png");
    buzzyTexture.loadFromFile("graphics/Buzzy_blue.png");
    enemyTexture1.loadFromFile("graphics/bulldog.png");
    enemyTexture2.loadFromFile("graphics/clemson_tigers.png");
    enemyTexture3.loadFromFile("graphics/rolltide.png");
    playerLaserTexture.loadFromFile("graphics/player_laser.png");
    enemyLaserTexture.loadFromFile("graphics/enemy_laser.png");

}

void ECE_Defender::setupBackgroundSprite()
{   
    // Scale background to fit screen
    backgroundSize = backgroundTexture.getSize();
    float scaleX = (float) windowSize.x / backgroundSize.x; // use Y to keep it squareish and not stretch
    float scaleY = (float) windowSize.y / backgroundSize.y;

    // setup sprite
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(scaleX, scaleY);
    backgroundSprite.setPosition(0, 0);
}

void ECE_Defender::setupStartScreenSprite()
{   
    // Scale startScreen to fit screen
    startScreenSize = startScreenTexture.getSize();
    float scaleX = (float) windowSize.x / startScreenSize.x; // use Y to keep it squareish and not stretch
    float scaleY = (float) windowSize.y / startScreenSize.y;

    // setup sprite
    startScreenSprite.setTexture(startScreenTexture);
    startScreenSprite.setScale(scaleX, scaleY);
    startScreenSprite.setPosition(0, 0);
}

void ECE_Defender::setupGameOverSprite()
{   
    // Scale gameOverScreen to fit screen
    gameOverScreenSize = gameOverScreenTexture.getSize();
    float scaleX = (float) windowSize.x / gameOverScreenSize.x; // use Y to keep it squareish and not stretch
    float scaleY = (float) windowSize.y / gameOverScreenSize.y;

    // setup sprite
    gameOverScreenSprite.setTexture(gameOverScreenTexture);
    gameOverScreenSprite.setScale(scaleX, scaleY);
    gameOverScreenSprite.setPosition(0, 0);
}

void ECE_Defender::setupGameWonSprite()
{   
    // Scale gameWonScreen to fit screen
    gameWonScreenSize = gameWonScreenTexture.getSize();
    float scaleX = (float) windowSize.x / gameWonScreenSize.x; // use Y to keep it squareish and not stretch
    float scaleY = (float) windowSize.y / gameWonScreenSize.y;

    // setup sprite
    gameWonScreenSprite.setTexture(gameWonScreenTexture);
    gameWonScreenSprite.setScale(scaleX, scaleY);
    gameWonScreenSprite.setPosition(0, 0);
}

/*
------------------------------------------------------------------------------------------------
Functions to handle game state
------------------------------------------------------------------------------------------------
*/

void ECE_Defender::initializeGame()
{
    // Reset game variables
    level = 1; // Future use
    score = 0; // Future use
    buzzy.setLives(3);
    maxEnemies = 20;
    totalEnemies = 0;
    enemies.clear();
    playerLaserBlasts.clear();
    enemyLaserBlasts.clear();
    gameOver = false;
    gamePaused = false;

    // Reset Buzzy's position
    buzzy.setStartPosition();
}

/*
------------------------------------------------------------------------------------------------
Helper functions for game display
------------------------------------------------------------------------------------------------
*/

void ECE_Defender::drawGameObjects()
{
    // Draw background
    this->draw(backgroundSprite);
    // Draw Buzzy
    this->draw(buzzy);
    // Draw all player lasers
    for (auto laser = playerLaserBlasts.begin(); laser != playerLaserBlasts.end(); ++laser)
    {
        this->draw(*laser);
    }
    // Draw all enemy lasers
    for (auto laser = enemyLaserBlasts.begin(); laser != enemyLaserBlasts.end(); ++laser)
    {
        this->draw(*laser);
    }
    // Draw all enemies
    for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
    {
        this->draw(*enemy);
    }
}

/*
------------------------------------------------------------------------------------------------
Functions to update game objects
------------------------------------------------------------------------------------------------
*/

void ECE_Defender::updateBuzzy()
{
    buzzy.update(frameTime); // update Buzzy's position
}

void ECE_Defender::updateEnemies()
{
    // Update enemies on the next frame after they were initialized
    for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
    {
        enemy->update(frameTime); // update enemy position
    }
    spawnEnemy(); // Spawn new enemies
}

void ECE_Defender::updateLasers()
{
    updatePlayerLasers(); // Update player lasers
    updateEnemyLasers(); // Update enemy lasers
    firePlayerLaser(); // handle player firing laser
    fireEnemyLasers(); // Handle enemies firing lasers
}

void ECE_Defender::updatePlayerLasers()
{
    // Update player laser blasts on the next frame after they were initialized
    for (auto laser = playerLaserBlasts.begin(); laser != playerLaserBlasts.end(); )
    {
        laser->update(frameTime);

        if (laser->boundaryDetected())
        {
            // Remove laser if it goes out of bounds
            laser = playerLaserBlasts.erase(laser);
        }
        else
        {
            // erase automatically advances iterator so iterate here
            ++laser;
        }
    }
}

void ECE_Defender::updateEnemyLasers()
{
    // Update player laser blasts on the next frame after they were initialized
    for (auto laser = enemyLaserBlasts.begin(); laser != enemyLaserBlasts.end(); )
    {
        laser->update(frameTime);
        if (laser->boundaryDetected())
        {
            // Remove laser if it goes out of bounds
            laser = enemyLaserBlasts.erase(laser);
        }
        else
        {
            // erase automatically advances iterator so iterate here
            ++laser;
        }
    }
}

/*
------------------------------------------------------------------------------------------------
Functions to handle collisions
------------------------------------------------------------------------------------------------
*/

void ECE_Defender::handleCollisions()
{
    handlePlayerCollisions(); // check if buzzy has run into an enemy
    handleEnemyCollisions(); // check if any lasers have hit the enemy
    handleLaserCollisions(); // check if any lasers have hit buzzy
}

void ECE_Defender::handlePlayerCollisions()
{
    // Check for collisions between enemies and player
    auto enemy = enemies.begin();
    while (enemy != enemies.end())
    {
        if (enemy->collisionDetected(buzzy))
        {
            enemies.erase(enemy);
            buzzy.setLives(0);
            if (buzzy.getLives() <= 0)
            {
                gameOver = true;
            }
            break;
        }
        else
        {
            ++enemy; // advance iterator if no collision
        }
    }
}

void ECE_Defender::handleEnemyCollisions()
{
    // Check for collisions between laser blasts and enemies
    auto laser = playerLaserBlasts.begin();
    while (laser != playerLaserBlasts.end())
    {
        enemyHit = false;
        auto enemy = enemies.begin();
        while (enemy != enemies.end() && !enemyHit)
        {
            if (laser->collisionDetected(*enemy)) // If player laser hits enemy
            {
                // Remove the enemy and the laser
                enemy = enemies.erase(enemy);
                laser = playerLaserBlasts.erase(laser);
                score += 1; // Increase score for destroying an enemy
                enemyHit = true; // Exit inner loop since laser is gone
                break;
            }
            else
            {
                ++enemy; // advance iterator if enemy not erased
            }
        }
        if (!enemyHit)
        {
            ++laser; // advance iterator if laser was not erased
        }
    }
}

void ECE_Defender::handleLaserCollisions()
{
    // Check for collisions between laser blasts and buzzy
    auto laser = enemyLaserBlasts.begin();
    while (laser != enemyLaserBlasts.end())
    {
        if (laser->collisionDetected(buzzy)) // If enemy laser hits player
        {
            if (laser->collisionDetected(buzzy))
            {
                // Remove the laser and lose a life
                laser = enemyLaserBlasts.erase(laser);
                buzzy.setLives(buzzy.getLives() - 1);
                if (buzzy.getLives() <= 0)
                {
                    gameOver = true;
                }
            }
            
        }
        else
        {
            ++laser; // advance iterator if laser was not erased
        }
    }
}

/*
------------------------------------------------------------------------------------------------
Helper functions for game objects
------------------------------------------------------------------------------------------------
*/

void ECE_Defender::spawnEnemy()
{
    // Spawn an enemy at random intervals so they don't just come in a even line
    if (totalEnemies < maxEnemies && rand() % 100 < 15) // 15% chance to spawn each frame
    {
        // Only spawn if there are no enemies or the last enemy has cleared the spawn boundary
        if (enemies.empty() || enemies.back().spawnBoundaryClear())
        {
            if (totalEnemies < 20)
            {
                // Randomly select one of two enemy textures
                int enemyType = rand() % 2;
                ECE_Enemy newEnemy(enemyTexture1, false, windowSize); // Default initialization
                switch (enemyType)
                {
                case 0:
                    newEnemy = ECE_Enemy(enemyTexture1, false, windowSize);
                    break;
                case 1:
                    newEnemy = ECE_Enemy(enemyTexture2, false, windowSize);
                    break;
                default:
                    newEnemy = ECE_Enemy(enemyTexture1, false, windowSize); // Fallback
                    break;
                }
                enemies.push_back(newEnemy);
                totalEnemies++;
            }
            else if (totalEnemies == 20 & enemies.empty() && bossReady)
            {
                if (bossClock.getElapsedTime() > bossCooldown)
                {
                    ECE_Enemy newEnemy(enemyTexture3, true, windowSize);
                    enemies.push_back(newEnemy);
                    totalEnemies++;
                }

            }

            if (totalEnemies == 20 && !bossReady)
            {
                bossReady = true;
                bossClock.restart();
            }
        }
    }
}

void ECE_Defender::firePlayerLaser()
{
    // Check if player can fire a laser
    if (buzzy.fireLaser())
    {
        // Create a new laser blast at Buzzy's position
        ECE_LaserBlast playerLaser(playerLaserTexture, buzzy, false, windowSize);
        playerLaserBlasts.push_back(playerLaser);
    }
}

void ECE_Defender::fireEnemyLasers()
{
    // Check if any enemies can fire a laser
    for (auto& enemy : enemies)
    {
        if (enemy.fireLaser() && rand() % 100 < 10) // 10% chance to fire each frame
        {
            // Create a new laser blast at the enemy's position
            ECE_LaserBlast enemyLaser(enemyLaserTexture, enemy, true, windowSize);
            enemyLaserBlasts.push_back(enemyLaser);
        }
    }
}
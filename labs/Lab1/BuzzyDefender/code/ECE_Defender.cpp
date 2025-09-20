#include <iostream>

#include "ECE_Defender.h"

using namespace sf;
using namespace std;

ECE_Defender::ECE_Defender()
{
    vm = VideoMode(1920, 1080); // Set window size
    this->create(vm, "Buzzy Defender", Style::Default); // Create the window

    windowSize = this->getSize(); // Get the actual window size

    loadTextures(); // Load all textures
    setupBackground(); // Setup the background

    // Initialize player
    buzzy.setupBuzzy(buzzyTexture, windowSize);

    // Initialize game variables
    level = 1;
    score = 0;
    lives = 3;
    maxEnemies = 20;
    totalEnemies = 0;
    gameOver = false;
}

void ECE_Defender::refreshDisplay()
{
    this->clear();
    this->draw(backgroundSprite);
    this->draw(buzzy);
    for (auto laser = laserBlasts.begin(); laser != laserBlasts.end(); ++laser)
    {
        this->draw(*laser);
    }
    this->display();
}

void ECE_Defender::updateScene()
{
    // Update buzzy based on user input
    buzzy.update();

    updateLasers(); // Update all laser blasts
    updateEnemies(); // Update all enemies
    spawnEnemy(); // Spawn new enemies
    firePlayerLaser(); // Fire player laser if spacebar is pressed
    fireEnemyLasers(); // Fire enemy lasers if their cooldown has passed
    handleCollisions(); // Handle all collisions between objects
}

Vector2u ECE_Defender::getWindowSize()
{
    return windowSize;
}

void ECE_Defender::loadTextures()
{
    // Load all textures from file
    backgroundTexture.loadFromFile("graphics/background.png");
    buzzyTexture.loadFromFile("graphics/Buzzy_blue.png");
    enemyTexture.loadFromFile("graphics/enemyRed1.png");
    playerLaserTexture.loadFromFile("graphics/player_laser.png");
    enemyLaserTexture.loadFromFile("graphics/enemy_laser.png");

}

void ECE_Defender::setupBackground()
{   
    // Scale background to fit screen
    backgroundSize = backgroundTexture.getSize();
    float scaleX = (float) windowSize.x / backgroundSize.x;
    float scaleY = (float) windowSize.y / backgroundSize.y;

    // Create sprite
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(scaleX, scaleY);
    backgroundSprite.setPosition(0, 0);
}

void ECE_Defender::spawnEnemy()
{
    // Spawn an enemy at random intervals so they don't just come in a even line
    if (totalEnemies < maxEnemies && rand() % 100 < 2) // 2% chance to spawn each frame
    {
        // Only spawn if there are no enemies or the last enemy has cleared the spawn boundary
        if (enemies.empty() || enemies.back().spawnBoundaryClear())
        {
            ECE_Enemy newEnemy(enemyTexture, windowSize);
            enemies.push_back(newEnemy);
            totalEnemies++;
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
        laserBlasts.push_back(playerLaser);
    }
}

// CHECK THIS FUNCTION FOR CORRECTNESS!!
void ECE_Defender::fireEnemyLasers()
{
    // Check if any enemies can fire a laser
    for (auto& enemy : enemies)
    {
        if (enemy.fireLaser())
        {
            // Create a new laser blast at the enemy's position
            ECE_LaserBlast enemyLaser(enemyLaserTexture, enemy, true, windowSize);
            laserBlasts.push_back(enemyLaser);
        }
    }
}

void ECE_Defender::updateLasers()
{
    // Update laser blasts on the next frame after they were initialized
    for (auto laser = laserBlasts.begin(); laser != laserBlasts.end(); )
    {
        laser->update();
        if (laser->boundaryDetected())
        {
            // Remove laser if it goes out of bounds
            laser = laserBlasts.erase(laser);
        }
        else
        {
            // erase automatically advances iterator so iterate here
            ++laser;
        }
    }
}

// CHECK THIS FUNCTION FOR CORRECTNESS!!
void ECE_Defender::updateEnemies()
{
    // Update enemies on the next frame after they were initialized
    for (auto enemy = enemies.begin(); enemy != enemies.end(); )
    {
        enemy->update();
        if (enemy->getPosition().y > windowSize.y) // If enemy goes off bottom of screen
        {
            // Remove enemy and lose a life
            enemy = enemies.erase(enemy);
            lives--;
            if (lives <= 0)
            {
                gameOver = true;
                cout << "Game Over! Final Score: " << score << endl;
            }
        }
        else
        {
            // erase automatically advances iterator so iterate here
            ++enemy;
        }
    }
}

// CHECK THIS FUNCTION FOR CORRECTNESS!!
void ECE_Defender::handleCollisions()
{
    // Check for collisions between laser blasts and enemies or player
    for (auto laser = laserBlasts.begin(); laser != laserBlasts.end(); )
    {
        bool laserErased = false; // Flag to track if the laser has been erased

        if (laser->getSpeed() < 0) // Player laser
        {
            for (auto enemy = enemies.begin(); enemy != enemies.end(); )
            {
                if (laser->collisionDetected(*enemy))
                {
                    // Remove both the laser and the enemy
                    enemy = enemies.erase(enemy);
                    laser = laserBlasts.erase(laser);
                    score += 100; // Increase score for destroying an enemy
                    laserErased = true; // Set flag to true since the laser has been erased
                    break; // Exit the inner loop since the laser is gone
                }
                else
                {
                    ++enemy;
                }
            }
        }
        else // Enemy laser
        {
            if (laser->collisionDetected(buzzy))
            {
                // Remove the laser and lose a life
                laser = laserBlasts.erase(laser);
                lives--;
                if (lives <= 0)
                {
                    gameOver = true;
                    cout << "Game Over! Final Score: " << score << endl;
                }
                laserErased = true; // Set flag to true since the laser has been erased
            }
        }

        if (!laserErased)
        {
            ++laser; // Only advance iterator if the laser wasn't erased
        }
    }
}



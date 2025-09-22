/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/02/2025
Date Last Modified: 09/21/2025

Description:

Lab 1

This is the source file for the ECE_Defender class and implements all functions
and maintains all variables defined in ECE_Defender.h.
This class is derived from the SFML Window class.
ECE_Defender manages all game objects, state, and logic. 

*/

#include "ECE_Defender.h"

using namespace sf;
using namespace std;

/*
This is the constructor for ECE_Defender. This function initializes the window
and initializes all game logic variables. Calls other helper functions to initialize
different aspects of the game.

Arguments:
    N/A

Return Values:
    ECE_Defender
*/
ECE_Defender::ECE_Defender()
{
    vm = VideoMode(1280, 720); // Set window size
    this->create(vm, "Buzzy Defender", Style::Default); // Create the window

    windowSize = this->getSize(); // Get the actual window size

    // set frame rate cap
    //this->setFramerateLimit(60);

    // game state flags
    gamePaused = true;
    gameOver = false;
    gameWon = false;
    gameClockStarted = false;
    bossReady = false;

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

/*
This function refreshes the entire display by clearing the window,
drawing Sprites determined by game state, and finally displaying all Sprites.

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Defender::refreshDisplay()
{
    // clear the window
    this->clear();

    if (isGamePaused()) // show start screen
    {
        this->draw(startScreenSprite);
    }
    else if (isGameOver()) // show game over screen
    {
        this->draw(gameOverScreenSprite);
    }
    else if (isGameWon()) // show YOU WON!! screen
    {
        this->draw(gameWonScreenSprite);
    }
    else
    {
        drawGameObjects(); // draw all game objects when game is in play
    }

    // Display updated frame
    this->display();
}

/*
This function updates position of all objects currently in game.
Calls several helper functions to keep the code looking a little cleaner.

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Defender::updateScene(Time lastFrameTime)
{

    updateBuzzy(lastFrameTime); // Update buzzy
    updateEnemies(lastFrameTime); // Update all enemies
    updateLasers(lastFrameTime); // Update all laser blasts
    handleCollisions(); // handle all collisions
}

/*
------------------------------------------------------------------------------------------------
Functions to handle game state
------------------------------------------------------------------------------------------------
*/

/*
This function initializes the game once the player starts the game. 
Also used when the game is restarted after a win or loss.

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Defender::startGame()
{
    // Reset game variables
    level = 1; // future use
    score = 0; // future use
    buzzy.setLives(3);
    maxEnemies = 21;
    totalEnemies = 0;
    // clear all lists of game objects
    enemies.clear();
    topRowEnemies.clear();
    playerLaserBlasts.clear();
    enemyLaserBlasts.clear();
    //reset game state flags
    gamePaused = false;
    gameOver = false;
    gameWon = false;
    gameClockStarted = false;
    bossReady = false;
    
    // Reset Buzzy's position
    buzzy.setStartPosition();
}

/*
This function puts the game into the pause state so the 
start screen is displayed.

Arguments:
    N/A

Return Values:
    void
*/
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

/*
This function checks if the game is over determined by number of lives
the player has left.

Arguments:
    N/A

Return Values:
    bool - flag that's true if game is over, false if not
*/
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

/*
This function checks if the game has been won determined by number of enemies left
out of total enemies that will spawn.

Arguments:
    N/A

Return Values:
    bool - flag that's true if game has been won, false if not
*/
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

/*
This function checks if the game has been paused

Arguments:
    N/A

Return Values:
    bool - flag that's true if game is paused, false if not
*/
bool ECE_Defender::isGamePaused()
{
    return gamePaused;
}

/*
This function returns the size of the game window.

Arguments:
    N/A

Return Values:
    Vector2u - size of window
*/
Vector2u ECE_Defender::getWindowSize()
{
    return windowSize;
}

/*
------------------------------------------------------------------------------------------------
Functions to initialize textures and fullscreen sprites
------------------------------------------------------------------------------------------------
*/

/*
This function loads all textures so they are all loaded once. Called as part of ECE_Defender initialization.

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Defender::loadTextures()
{
    // Load all textures from file
    backgroundTexture.loadFromFile("graphics/background.png"); // Background
    startScreenTexture.loadFromFile("graphics/Start_Screen.png"); // Start Screen
    gameOverScreenTexture.loadFromFile("graphics/gameover.png"); // Game Over Screen
    gameWonScreenTexture.loadFromFile("graphics/success.png"); // You Won!! Screen
    buzzyTexture.loadFromFile("graphics/Buzzy_blue.png"); // Buzzy itself
    enemyTexture1.loadFromFile("graphics/bulldog.png"); // Georgia Bulldogs
    enemyTexture2.loadFromFile("graphics/clemson_tigers.png"); // Clemson Tigers
    enemyTexture3.loadFromFile("graphics/rolltide.png"); // Alabama Crimson Tide (used to be huge rivals in the Bear Bryant days)
    // The above is my alma mater (undergrad) and I'm very aware we're the bad guys in college football
    playerLaserTexture.loadFromFile("graphics/player_laser.png"); // Player Laser. Green for good guys
    enemyLaserTexture.loadFromFile("graphics/enemy_laser.png"); // Enemy Laser. Red for bad guys

}

/*
This function sets up the background sprite. 

Arguments:
    N/A

Return Values:
    void
*/
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

/*
This function sets up the start screen sprite. 

Arguments:
    N/A

Return Values:
    void
*/
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

/*
This function sets up the game over screen sprite. 

Arguments:
    N/A

Return Values:
    void
*/
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

/*
This function sets up the game won sprite. 

Arguments:
    N/A

Return Values:
    void
*/
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
Helper functions for game display
------------------------------------------------------------------------------------------------
*/

/*
This function collects all calls to draw all game objects. Keeps code a bit cleaner. 

Arguments:
    N/A

Return Values:
    void
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

/*
This function calls Buzzy's update function 

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_Defender::updateBuzzy(Time lastFrameTime)
{
    buzzy.update(lastFrameTime); // update Buzzy's position
}

/*
This function calls all enemies' update functions

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_Defender::updateEnemies(Time lastFrameTime)
{
    // Update enemies on the next frame after they were initialized
    for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
    {
        enemy->update(lastFrameTime); // update enemy position
        if (enemy->inTopRow())
        {
            topRowEnemies.push_back(*enemy);
        }
    }
    spawnEnemy(); // Spawn new enemies
}

/*
This function calls all lasers' update functions via two helper functions.
One for lasers fired by player and one for lasers fired by enemies. 
Also calls functions to handle lasers being fired.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_Defender::updateLasers(Time lastFrameTime)
{
    updatePlayerLasers(lastFrameTime); // Update player lasers
    updateEnemyLasers(lastFrameTime); // Update enemy lasers
    firePlayerLaser(); // handle player firing laser
    fireEnemyLasers(); // Handle enemies firing lasers
}

/*
This function calls update function for all lasers fired by player.
If the laser hits the edge of the screen, it is erased from the list container.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_Defender::updatePlayerLasers(Time lastFrameTime)
{
    // Update player laser blasts on the next frame after they were initialized
    for (auto laser = playerLaserBlasts.begin(); laser != playerLaserBlasts.end(); )
    {
        laser->update(lastFrameTime);

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

/*
This function calls update function for all lasers fired by enemy.
If the laser hits the edge of the screen, it is erased from the list container.

Arguments:
    lastFrameTime - used to scale the distance the object is moved each time based time since last frame update

Return Values:
    void
*/
void ECE_Defender::updateEnemyLasers(Time lastFrameTime)
{
    // Update player laser blasts on the next frame after they were initialized
    for (auto laser = enemyLaserBlasts.begin(); laser != enemyLaserBlasts.end(); )
    {
        laser->update(lastFrameTime);
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

/*
This function collects all helper function calls to handle collision detection for each game object.

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Defender::handleCollisions()
{
    if (!topRowEnemies.empty())
    {   // don't bother checking if there no enemies in line with the player
        handlePlayerCollisions(); // check if buzzy has run into an enemy
    }
    handleEnemyCollisions(); // check if any lasers have hit the enemy
    handleLaserCollisions(); // check if any lasers have hit buzzy
}

/*
This function checks for collisions between player and each enemy. If collision detected,
buzzy's lives are reduced to 0 and the game is over.

Arguments:
    N/A

Return Values:
    void
*/
void ECE_Defender::handlePlayerCollisions()
{
    // Check for collisions between enemies and player
    auto enemy = topRowEnemies.begin();
    while (enemy != topRowEnemies.end())
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

/*
This function checks for collisions between each enemy and lasers fired by the player. 
If collision detected, the enemy is erased from the list. The score is incremented, although
that does nothing for now.

Arguments:
    N/A

Return Values:
    void
*/
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

/*
This function checks for collisions between each laser fired by enemies and the player. 
If collision detected, the player loses a life. if lives reach 0, the game is over.

Arguments:
    N/A

Return Values:
    void
*/
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

/*
This function handles spawning enemies to attack the player until it reaches the enemy limit. 
It only spawns an enemy if the list is empty or the spawn location is clear. 
It randomizes the spawning of an enemy so they don't come at regular intervals. It also randomly
chooses between textures when spawning an enemy.
For the last enemy, it spawns a boss with it's own unique texture that moves a bit faster. 
The boss is spawned once all other enemies are defeated.
It manages a list container to track all enemies in game.

Arguments:
    N/A

Return Values:
    void
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
                    newEnemy = ECE_Enemy(enemyTexture1, false, windowSize); // Georgia Bulldog
                    break;
                case 1:
                    newEnemy = ECE_Enemy(enemyTexture2, false, windowSize); // Clemson Tigers
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

/*
This function checks checks if the laser can be fired by calling Buzzy's fireLaser function.
Buzzy handles the user input and fireability.
Manages a list container of lasers fired by player.

Arguments:
    N/A

Return Values:
    void
*/
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

/*
This function checks checks if the laser can be fired by calling the enemy's fireLaser function.
The enemy handles the user input and fireability.
Manages a list container of lasers fired by the enemy.
It iterates through each enemy and each enemy has a random chance to fire.

Arguments:
    N/A

Return Values:
    void
*/
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
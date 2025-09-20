#pragma once

#include <list>

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "ECE_Buzzy.h"
#include "ECE_LaserBlast.h"

using namespace sf;
using namespace std;

class ECE_Defender: public RenderWindow
{
public:
    ECE_Defender();
    void refreshDisplay();
    void updateScene();
    Vector2u windowSize;
private:
    void loadTextures();
    void setupBackground();
    void setupBuzzy();
    VideoMode vm;
    Texture backgroundTexture;
    Texture buzzyTexture;
    Texture enemyTexture;
    Texture playerLaserTexture;
    Texture enemyLaserTexture;
    Sprite backgroundSprite;
    Vector2u backgroundSize;
    ECE_Buzzy buzzy;
    list<ECE_LaserBlast> laserBlasts;
    Clock fireClock;
    Time fireCooldown;
};
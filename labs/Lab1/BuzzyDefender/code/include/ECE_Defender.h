#ifndef ECE_DEFENDER_H
#define ECE_DEFENDER_H

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "ECE_Buzzy.h"

using namespace sf;
using namespace std;

class ECE_Defender: public RenderWindow
{
public:
    ECE_Defender();
    void refreshDisplay();
    void updateScene();
    ECE_Buzzy buzzy;
    Vector2u windowSize;
private:
    void setupBackground();
    VideoMode vm;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Vector2u backgroundSize;
};

#endif // ECE_DEFENDER_H
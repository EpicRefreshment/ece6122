/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/25/2025
Date Last Modified: 11/04/2025

Description:

Final Project

Header file for the GlobalControlPanel class.
This class encapsulates all logic and GUI elements for the
top control panel (transport, BPM, logo, etc.).
*/

#pragma once

#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "SequencerEngine.h"

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

class GlobalControlPanel
{
public:
    GlobalControlPanel(RenderWindow& window, SequencerEngine& engine, const Font& font, Vector2f panelSize, Vector2f panelPos);
    
    void handleKeyboard(Event event);
    void handleMouse(Event event, float mousePosX, float mousePosY);
    void handleText(Event event);

    void update();
    void draw();

    FloatRect getGlobalBounds();

private:
    void setupText(Text& text, const String& value, Vector2f position);

    void setBPM();
    void setBPMFromString();

    RenderWindow& window;
    SequencerEngine& engine;
    const Font& font;

    int bpm;
    string bpmString;
    string bpmInputString;

    int editBPM;
    
    RectangleShape panelBackground;
    RectangleShape playButton, pauseButton, stopButton;
    RectangleShape bpmBox, bpmUpButton, bpmDownButton;
    Text logoText, nameText;
    Text playText, pauseText, stopText;
    Text bpmLabelText, bpmValueText, bpmUpText, bpmDownText;
};
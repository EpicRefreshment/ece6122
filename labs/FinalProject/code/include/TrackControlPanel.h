/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/06/2025
Date Last Modified: 11/06/2025

Description:

Final Project

Header file for the TrackControlPanel class.
This class encapsulates all logic and GUI elements for the
side panel that contains controls for each track (e.g., track length, tempo division).
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "SequencerEngine.h" // Need engine to post commands
#include "SeqTrack.h"

using namespace std;
using namespace sf;

class TrackControlPanel
{
public:
    TrackControlPanel(RenderWindow& window, SequencerEngine& engine, const Font& font, const vector<SeqTrack*>& tracks, Vector2f panelSize, Vector2f panelPos);

    void handleMouse(Event event, float mousePosX, float mousePosY);
    void update();
    void draw();

    FloatRect getGlobalBounds();

private:
    void setupText(Text& text, const String& value, Vector2f position, int size);
    void initShapes();
    void initDropdownItems();
    void resizeUIVectors();
    void initTrackBackground(int index, float trackHeight);
    void initDropdownButtons(int index, float trackHeight);
    void initParameterControls(int index, float trackHeight);
    void initMuteSoloControls(int index, float trackHeight);
    void initGenerateControls(int index, float trackHeight);
    void updateTrackText(int trackIndex);
    void handleDropdown(Event event, float mousePosX, float mousePosY);

    RenderWindow& window;
    SequencerEngine& engine;
    const Font& font;
    const vector<SeqTrack*>& tracks;

    int numTracks;
    int numModes;
    int activeDropdown;

    RectangleShape panelBackground;
    RectangleShape dropdownBackground;

    vector<Text> trackLabels;
    vector<Text> dropdownItems;
    vector<Text> modeText;
    vector<Text> param1Text, param2Text, param3Text, param4Text;
    vector<Text> param1UpText, param2UpText, param3UpText, param4UpText;
    vector<Text> param1DownText, param2DownText, param3DownText, param4DownText;
    vector<Text> muteText, soloText;
    vector<Text> generateText, regenRateText;
    vector<Text> regenRateUpText, regenRateDownText;
    
    vector<RectangleShape> dropdownButtons;
    vector<RectangleShape> param1UpButtons, param1DownButtons;
    vector<RectangleShape> param2UpButtons, param2DownButtons;
    vector<RectangleShape> param3UpButtons, param3DownButtons;
    vector<RectangleShape> param4UpButtons, param4DownButtons;
    vector<RectangleShape> muteButtons, soloButtons;
    vector<RectangleShape> generateButtons;
    vector<RectangleShape> regenRateUpButtons, regenRateDownButtons;

    vector<RectangleShape> trackBackgrounds;
};

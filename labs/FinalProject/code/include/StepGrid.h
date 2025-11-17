/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/01/2025
Date Last Modified: 11/04/2025

Description:

Final Project

Header file for the StepSequencePanel class.
This class encapsulates all logic and GUI elements for the
main 8x16 step sequencer grid.
*/

#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "SequencerEngine.h"
#include "SeqTrack.h"
#include "SampleControlPanel.h"

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

class StepGrid
{
public:
    StepGrid(RenderWindow& window, SequencerEngine& engine, const Font& font, const vector<SeqTrack*>& tracks,
             SampleControlPanel& sampleCtrlPanel, Vector2f panelSize, Vector2f panelPos);

    void handleMouse(Event event, float mousePosX, float mousePosY);

    void draw();

    FloatRect getGlobalBounds();

private:
    void initShapes();

    RenderWindow& window;
    SequencerEngine& engine;
    const Font& font;
    const std::vector<SeqTrack*>& tracks;
    SampleControlPanel& sampleCtrlPanel;

    float padding;
    float stepWidth;
    float stepHeight;

    int numTracks;
    int numSteps;

    std::vector<RectangleShape> stepShapes;

    RectangleShape panelBackground;
};
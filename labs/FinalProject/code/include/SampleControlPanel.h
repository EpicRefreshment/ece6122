/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/04/2025
Date Last Modified: 11/04/2025

Description:

Final Project

Header file for the GlobalControlPanel class.
This class encapsulates all logic and GUI elements for the
top control panel (transport, BPM, logo, etc.).
*/

#pragma once

#include <iostream>
#include <filesystem>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <SeqTrack.h>

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

namespace fs = std::filesystem; 

class SampleControlPanel
{
public:
    SampleControlPanel(RenderWindow& window, const Font& font, const vector<SeqTrack*>& tracks, Vector2f panelSize, Vector2f panelPos);

    void handleMouse(Event event, float mousePosX, float mousePosY);
    void update();
    void draw();

    FloatRect getGlobalBounds();

private:
    void setupText(Text& text, const String& value, Vector2f position);
    void initShapes();

    void loadSamples(const string& directory);
    
    void generateWaveform(SoundBuffer& buffer);
    void displayWaveform(int trackIndex, int bufferIndex);

    RenderWindow& window;
    const Font& font;
    const vector<SeqTrack*>& tracks;
    int numTracks;

    RectangleShape panelBackground;

    vector<SoundBuffer> sampleBuffers;
    vector<VertexArray> sampleWaveforms;

    // for display
    vector<Text> trackLabels;
    vector<Text> sampleNameTexts;
    vector<VertexArray> waveformDisplays;

    int activeDropdown;
    vector<RectangleShape> dropdownButtons;
    vector<Text> dropdownItems;
    RectangleShape dropdownBackground;

    vector<RectangleShape> trackBackgrounds;
};
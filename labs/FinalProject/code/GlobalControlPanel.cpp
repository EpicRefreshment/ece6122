/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/25/2025
Date Last Modified: 11/04/2025

Description:

Final Project

Source file for the GlobalControlPanel class.
Implements all setup, event handling, and drawing for the
top control panel.
*/

#include "GlobalControlPanel.h"

using namespace sf;
using namespace std;

GlobalControlPanel::GlobalControlPanel(RenderWindow& window, SequencerEngine& engine, const Font& font, Vector2f panelSize, Vector2f panelPos)
    : window(window), engine(engine), font(font)
{

    // Panel Background
    panelBackground.setSize(panelSize);
    panelBackground.setFillColor(Color(50, 50, 50));
    panelBackground.setPosition(panelPos);

    // Logo and app name text
    setupText(logoText, "Wolford Audio Research", {10, 30});
    setupText(nameText, "Multimode Sequencer", {10, 44});

    // Transport Buttons
    playButton.setSize({60, 25});
    playButton.setFillColor(Color::Black);
    playButton.setOutlineColor(Color::White);
    playButton.setOutlineThickness(1);
    playButton.setPosition({(panelSize.x / 2 - 100), 25});

    pauseButton.setSize({60, 25});
    pauseButton.setFillColor(Color::Black);
    pauseButton.setOutlineColor(Color::White);
    pauseButton.setOutlineThickness(1);
    pauseButton.setPosition({(panelSize.x / 2 - 30), 25});

    stopButton.setSize({60, 25});
    stopButton.setFillColor(Color::Black);
    stopButton.setOutlineColor(Color::White);
    stopButton.setOutlineThickness(1);
    stopButton.setPosition({(panelSize.x / 2 + 40), 25});

    setupText(playText, "Play", {playButton.getPosition().x + 10, 29});
    setupText(pauseText, "Pause", {pauseButton.getPosition().x + 10, 29});
    setupText(stopText, "Stop", {stopButton.getPosition().x + 10, 29});

    // -- BPM Controls --
    editBPM = 0;
    bpm = engine.getBPM();
    bpmString = to_string(bpm);
    bpmInputString = bpmString;

    bpmBox.setSize({60, 30});
    bpmBox.setPosition({(panelSize.x / 2 + 240), 22});
    bpmBox.setFillColor(Color::Black);
    bpmBox.setOutlineColor(Color(80, 80, 80));
    bpmBox.setOutlineThickness(1.0f);

    bpmUpButton.setSize({20, 15});
    bpmUpButton.setPosition({(bpmBox.getPosition().x + 65), 22});
    bpmUpButton.setFillColor(Color::Black);
    bpmUpButton.setOutlineColor(Color(80, 80, 80));
    bpmUpButton.setOutlineThickness(1.0f);

    bpmDownButton.setSize({20, 15});
    bpmDownButton.setPosition({(bpmBox.getPosition().x + 65), 37});
    bpmDownButton.setFillColor(Color::Black);
    bpmDownButton.setOutlineColor(Color(80, 80, 80));
    bpmDownButton.setOutlineThickness(1.0f);

    setupText(bpmLabelText, "BPM:", {bpmBox.getPosition().x - 35, 28});
    setupText(bpmValueText, bpmString, {bpmBox.getPosition().x + 15, 28});
    setupText(bpmUpText, "+", {bpmUpButton.getPosition().x + 7, bpmUpButton.getPosition().y - 2});
    setupText(bpmDownText, "-", {bpmDownButton.getPosition().x + 7, bpmDownButton.getPosition().y - 2});
}

void GlobalControlPanel::handleKeyboard(Event event)
{
    if (event.key.code == Keyboard::Space)
    {
        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            engine.stop();
        }
        else if (engine.isPlaying())
        {
            engine.pause();
        }
        else
        {
            engine.play();
        }
    }
    else if (event.key.code == Keyboard::Escape)
    {
        window.close();
    }
}

void GlobalControlPanel::handleText(Event event)
{
    // --- Text Input Handling for BPM ---
    if (event.type == Event::TextEntered && editBPM)
    {
        // Backspace
        if (event.text.unicode == 8 && !bpmInputString.empty())
        {
            bpmInputString.pop_back();
        }
        // Enter Key
        else if (event.text.unicode == 13)
        {
            setBPMFromString();
            editBPM = 0;
        }
        // Numeric input
        else if (event.text.unicode >= '0' && event.text.unicode <= '9')
        {
            bpmInputString += static_cast<char>(event.text.unicode);
        }
    }
}

void GlobalControlPanel::handleMouse(Event event, float mousePosX, float mousePosY)
{
    // --- Panel Button Clicks ---
    if (playButton.getGlobalBounds().contains(mousePosX, mousePosY))
    {
        engine.play();
    }
    else if (pauseButton.getGlobalBounds().contains(mousePosX, mousePosY))
    {
        engine.pause();
    }
    else if (stopButton.getGlobalBounds().contains(mousePosX, mousePosY))
    {
        engine.stop();
    }
    else if (bpmUpButton.getGlobalBounds().contains(mousePosX, mousePosY))
    {
        bpm++;
        setBPM();
    }
    else if (bpmDownButton.getGlobalBounds().contains(mousePosX, mousePosY))
    {
        bpm--;
        setBPM();
    }
    // --- BPM Text Box Click ---
    else if (bpmBox.getGlobalBounds().contains(mousePosX, mousePosY))
    {
        editBPM = 1;
    }
    // --- Clicked outside BPM box ---
    else
    {
        if (editBPM)
        {
            editBPM = 0;
        }
    }
}

void GlobalControlPanel::update()
{
    // Update BPM Text
    if (editBPM)
    {
        bpmValueText.setString(bpmInputString + "_");
        bpmBox.setOutlineColor(Color::Cyan);
    }
    else
    {
        bpmValueText.setString(bpmString);
        bpmBox.setOutlineColor(Color(80, 80, 80));
    }

    // Update transport buttons
    playButton.setOutlineColor(engine.isPlaying() ? Color::Green : Color::White);
    pauseButton.setOutlineColor(engine.isPaused() ? Color::Yellow : Color::White);
    stopButton.setOutlineColor(engine.isStopped() ? Color::Red : Color::White);
}

void GlobalControlPanel::draw()
{   
    // -- Draw Panel Elements --
    window.draw(panelBackground);
    window.draw(logoText);
    window.draw(nameText);
    window.draw(playButton);
    window.draw(pauseButton);
    window.draw(stopButton);
    window.draw(playText);
    window.draw(pauseText);
    window.draw(stopText);
    window.draw(bpmLabelText);
    window.draw(bpmBox);
    window.draw(bpmValueText);
    window.draw(bpmUpButton);
    window.draw(bpmDownButton);
    window.draw(bpmUpText);
    window.draw(bpmDownText);
}

FloatRect GlobalControlPanel::getGlobalBounds()
{
    return panelBackground.getGlobalBounds();
}

// --- Private Helper Functions ---

void GlobalControlPanel::setupText(Text& text, const String& value, Vector2f position)
{
    text.setFont(font);
    text.setString(value);
    text.setCharacterSize(14);
    text.setFillColor(Color::White);
    text.setPosition(position);
}

void GlobalControlPanel::setBPM()
{
    if (bpm < 20)
    {
        bpm = 20;
    }
    else if (bpm > 200)
    {
        bpm = 200;
    }
    engine.setBpm(bpm);
    bpmString = to_string(bpm);
    bpmInputString = bpmString;
}

void GlobalControlPanel::setBPMFromString()
{
    // If the input string is empty, revert to the last valid BPM
    if (bpmInputString.empty())
    {
        bpmInputString = bpmString;
    }
    bpm = stoi(bpmInputString);
    bpmString = bpmInputString;
    setBPM();
}
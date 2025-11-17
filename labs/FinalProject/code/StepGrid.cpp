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

#include "StepGrid.h"

using namespace sf;
using namespace std;

StepGrid::StepGrid(RenderWindow& window, SequencerEngine& engine, const Font& font, const vector<SeqTrack*>& tracks,
                   SampleControlPanel& sampleCtrlPanel, Vector2f panelSize, Vector2f panelPos)
    : window(window), engine(engine), font(font), tracks(tracks), sampleCtrlPanel(sampleCtrlPanel)
{
    // Panel Background
    panelBackground.setSize(panelSize);
    panelBackground.setFillColor(Color(50, 50, 50));
    panelBackground.setPosition(panelPos);

    this->numTracks = static_cast<int>(tracks.size());
    numSteps = 16;

    padding = 5.0f;
    stepWidth = (panelBackground.getSize().x - (padding * (numSteps + 1))) / numSteps;
    stepHeight = (panelBackground.getSize().y - (padding * (numTracks + 1))) / numTracks;

    initShapes();
}

void StepGrid::draw()
{
    window.draw(panelBackground);
    
    // Get the global step from the engine for the playhead
    int activeGlobalStep = engine.getGlobalStep();

    // Redraw all shapes based on the current state
    for (int track = 0; track < numTracks; track++)
    {
        // current step will always be ahead by 1 when drawing
        int currentStep = tracks[track]->getCurrentStep();

        for (int step = 0; step < numSteps; step++)
        {
            int index = track * numSteps + step;

            if (step == currentStep && activeGlobalStep != -1)
            {
                // Active playhead: Use Yellow
                stepShapes[index].setOutlineColor(Color::Yellow); 
            }
            else
            {
                // Inactive playhead: Use Gray
                stepShapes[index].setOutlineColor(Color::Black);
            }
            
            if (step >= tracks[track]->getTrackLength())
            {
                // Step is beyond the track's length: Use a dim color
                stepShapes[index].setFillColor(Color(20, 20, 20));
            }
            else if (tracks[track]->isStepActive(step))
            {
                // Step is ON: Use Cyan
                stepShapes[index].setFillColor(Color::Cyan); 
            }
            else
            {
                // Step is OFF: Use Black
                stepShapes[index].setFillColor(Color::Black); 
            }
                
            window.draw(stepShapes[index]);
        }
    }
}

FloatRect StepGrid::getGlobalBounds()
{
    return panelBackground.getGlobalBounds();
}

void StepGrid::initShapes()
{
    // Initialize all the GUI shapes
    for (int track = 0; track < numTracks; track++)
    {
        for (int step = 0; step < numSteps; step++)
        {
            int index = track * numSteps + step;
            float posX = panelBackground.getPosition().x + padding + step * (stepWidth + padding);
            float posY = panelBackground.getPosition().y + padding + track * (stepHeight + padding);

            RectangleShape stepShape;
            stepShape.setSize(Vector2f(stepWidth, stepHeight));
            stepShape.setPosition(posX, posY);
            stepShape.setOutlineThickness(2.0f);
            stepShape.setOutlineColor(Color(80, 80, 80));
            stepShape.setFillColor(Color::Black);
            stepShapes.push_back(stepShape);
        }
    }
}

void StepGrid::handleMouse(Event event, float mousePosX, float mousePosY)
{
    // Find which shape was clicked
    for (int i = 0; i < stepShapes.size(); i++)
    {
        if (stepShapes[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            int track = i / numSteps;
            int step = i % numSteps;
            tracks[track]->toggleStep(step);
            break; // Found the shape
        }
    }
}
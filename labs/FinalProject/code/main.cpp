/*
Author: [Your Name/Gemini]
Class: ECE6122Q
Date Created: 11/03/2025
Date Last Modified: 11/04/2025

Description:

Multimode Sequencer Project

This is the main file for the step sequencer application.
It handles window creation, GUI setup, and the main application loop.
It owns the SequencerEngine (transport) and all SeqTrack (data) objects.
It is responsible for handling all input and drawing all GUI elements.
*/

#include <iostream>
#include <vector>

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "SequencerEngine.h"
#include "SeqTrack.h"

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

int main()
{
    // --- Application Constants ---
    int windowWidth = 800;
    int windowHeight = 600;
    int numTracks = 8;
    int numSteps = 16;
    float defaultBpm = 120.0f;

    // --- Window and GUI Setup ---
    RenderWindow window(VideoMode(windowWidth, windowHeight), "8-Track Step Sequencer");
    window.setFramerateLimit(60);

    float padding = 5.f;
    float stepHeight = (windowHeight - (padding * (numTracks + 1))) / numTracks;
    float stepWidth = (windowWidth - (padding * (numSteps + 1))) / numSteps;

    // We need one shape for every step on every track
    vector<RectangleShape> stepShapes(numTracks * numSteps);

    // Initialize all the GUI shapes
    for (int trk = 0; trk < numTracks; ++trk)
    {
        for (int stp = 0; stp < numSteps; ++stp)
        {
            int index = trk * numSteps + stp;
            float posX = padding + stp * (stepWidth + padding);
            float posY = padding + trk * (stepHeight + padding);

            stepShapes[index].setSize(Vector2f(stepWidth, stepHeight));
            stepShapes[index].setPosition(posX, posY);
            stepShapes[index].setOutlineThickness(2.f);
            stepShapes[index].setOutlineColor(Color(80, 80, 80));
            stepShapes[index].setFillColor(Color::Black); // Default off
        }
    }

    // --- Sequencer Initialization ---
    SequencerEngine engine;

    vector<SeqTrack> tracks(numTracks);

    // --- Main Application Loop ---
    while (window.isOpen())
    {
        // --- Event Handling ---
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            // Start/Stop with the space bar
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code == Keyboard::Space)
                {
                    if (engine.isPlaying())
                    {
                        engine.pause();
                    }
                    else
                    {
                        engine.play();
                    }
                }
                else if (event.key.code == Keyboard::Enter)
                {
                    engine.stop();
                }
                else if (event.key.code == Keyboard::Escape)
                {
                    window.close();
                }
            }
            
            // Toggle steps with mouse click
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                // Find which shape was clicked
                for (int i = 0; i < stepShapes.size(); i++)
                {
                    if (stepShapes[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    {
                        int track = i / numSteps;
                        int step = i % numSteps;
                        tracks[track].toggleStep(step);
                        cout << "Toggled Track: " << track << " Step: " << step << endl;
                        break; // Found the shape
                    }
                }
            }
        }

        // --- Logic Update ---
        // engine.update() returns true ONCE per 16th note "tick"
        if (engine.update())
        {
            // A tick has occurred, so we advance the playhead
            int currentStep = engine.getCurrentStep();

            // Trigger all tracks for this new step
            for (int i = 0; i < numTracks; ++i)
            {
                tracks[i].trigger(currentStep, i);
            }
        }

        // --- Drawing ---
        window.clear(Color(30, 30, 30)); // Dark grey background

        // Get the active step from the engine
        int activeStep = engine.getCurrentStep();

        // Redraw all shapes based on the current state
        for (int trk = 0; trk < numTracks; trk++)
        {
            for (int stp = 0; stp < numSteps; stp++)
            {
                int index = trk * numSteps + stp;

                // Set color based on state
                if (stp == activeStep)
                {
                    // Active playhead: Use Yellow
                    stepShapes[index].setFillColor(Color::Yellow); 
                }
                else if (tracks[trk].isStepActive(stp))
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
        
        window.display();
    }

    return 0;
}
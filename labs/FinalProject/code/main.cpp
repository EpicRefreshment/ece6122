/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/11/2025
Date Last Modified: 11/04/2025

Description:

Final Project

This is the main file for the step sequencer application.
It handles window creation, GUI setup, and the main application loop.
It owns the SequencerEngine (transport) and all SeqTrack (data) objects.
It is responsible for handling all input and drawing all GUI elements.
*/

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

// Include SFML libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include custom C++ libraries here
#include "SequencerEngine.h"
#include "SeqTrack.h"
#include "GlobalControlPanel.h"
#include "StepGrid.h"
#include "SampleControlPanel.h"
#include "ThreadPool.h"

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

int main()
{
    // Number of threads in the thread pool
    const size_t threadCount = 16;
    ThreadPool pool(threadCount);

    const float panelRatio = 1.0f / 5.0f;
    const float ratioDivisor = 5.0f;

    // GUI Configuration
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Multimode Step Sequencer");
    window.setFramerateLimit(60);

    Vector2u windowSize = window.getSize();
    float windowWidth = static_cast<float>(windowSize.x); // for readability
    float windowHeight = static_cast<float>(windowSize.y); // for readability

    Vector2f globalPanelSize(windowWidth, windowHeight / 10.0f);
    Vector2f stepGridSize(windowWidth * panelRatio * (ratioDivisor - 2), windowHeight - globalPanelSize.y);
    Vector2f sampleCtrlSize(windowWidth * panelRatio, windowHeight - globalPanelSize.y);

    Vector2f globalPanelPos(0, 0);
    Vector2f stepGridPos(windowWidth * panelRatio, globalPanelSize.y);
    Vector2f sampleCtrlPos(stepGridSize.x + stepGridPos.x, globalPanelSize.y);

    // --- Font Loading ---
    Font font;
    // Make sure to provide a valid path to a font file.
    // For example, create a "fonts" folder in your project.
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf")) {
        cerr << "Error loading font" << endl;
        return -1;
    }

    // --- Sequencer Initialization ---
    SequencerEngine engine;

    // --- Panel Initialization ---
    // Initialize all the tracks
    int numTracks = 8;
    int numSteps = 16;
    vector<SeqTrack> tracks;
    for (int i = 0; i < numTracks; i++)
    {
        tracks.push_back(SeqTrack(i, numSteps));
    }
    
    GlobalControlPanel globalPanel(window, engine, font, globalPanelSize, globalPanelPos);
    SampleControlPanel sampleCtrlPanel(window, font, tracks, sampleCtrlSize, sampleCtrlPos);
    StepGrid stepGrid(window, engine, font, tracks, sampleCtrlPanel, stepGridSize, stepGridPos);

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

            // Transport keyboard shortcuts
            if (event.type == Event::KeyReleased)
            {
                globalPanel.handleKeyboard(event);
            }
            
            // Handle mouse events
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                float mousePosX = static_cast<float>(mousePos.x);
                float mousePosY = static_cast<float>(mousePos.y);

                if (globalPanel.getGlobalBounds().contains(mousePosX, mousePosY))
                {
                    globalPanel.handleMouse(event, mousePosX, mousePosY);
                }
                else if (stepGrid.getGlobalBounds().contains(mousePosX, mousePosY))
                {
                    stepGrid.handleMouse(event, mousePosX, mousePosY);
                }
                else if (sampleCtrlPanel.getGlobalBounds().contains(mousePosX, mousePosY))
                {
                    sampleCtrlPanel.handleMouse(event, mousePosX, mousePosY);
                }
            }

            // Handle text events
            if (event.type == Event::TextEntered)
            {
                globalPanel.handleText(event);
            }
        }

        // Update sequencer
        globalPanel.update();
        sampleCtrlPanel.update();

        // --- Logic Update ---
        // engine.update() returns true ONCE per 16th note "tick"
        if (engine.update())
        {
            stepGrid.update();
        }

        // --- Drawing ---
        window.clear(Color(30, 30, 30)); // Dark grey background
        
        // Draw panels
        globalPanel.draw();
        stepGrid.draw();
        sampleCtrlPanel.draw();

        window.display();
    }

    return 0;
}
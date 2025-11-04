/*
Author: [Your Name/Gemini]
Class: ECE6122Q
Date Created: 11/03/2025
Date Last Modified: 11/04/2025

Description:

Multimode sequencer project

This is the header file for the SequencerEngine class.
This class manages the core timing and transport controls (play, stop, pause).
It functions as a metronome, providing a "tick" via its update() method
based on the BPM. It does not manage step state.
*/

#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class SequencerEngine
{
public:
    SequencerEngine();
    void play();
    void stop();
    void pause();
    bool update();
    void setBpm(float newBpm);
    bool isPlaying();
    int getCurrentStep();
private:
    bool playing;
    float bpm;
    int currentStep;

    sf::Clock clock;
    sf::Time stepTime; // Duration of one 16th note step
};
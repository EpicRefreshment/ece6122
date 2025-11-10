/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/15/2025
Date Last Modified: 11/04/2025

Description:

Final Project

This is the header file for the SequencerEngine class.
This class manages the core timing and transport controls (play, stop, pause).
It functions as a metronome, providing a "tick" via its update() method
based on the BPM. It does not manage step state.
*/

#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

class SequencerEngine
{
public:
    SequencerEngine();

    void play();
    void stop();
    void pause();
    bool update();

    int isPlaying();
    int isPaused();
    int isStopped();

    void setBpm(int bpm);

    int getBPM();
    int getCurrentStep();
private:
    int playing;
    int paused;
    int stopped;

    int bpm;
    int currentStep;

    Clock clock;
    Time stepTime; // Duration of one 16th note step
};
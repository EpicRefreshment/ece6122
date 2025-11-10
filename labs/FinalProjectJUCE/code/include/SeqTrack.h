/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/15/2025
Date Last Modified: 11/04/2025

Description:

Final Project

This is the header file for the SeqTrack class.
This class defines the properties and methods for a single sequencer track.
It is responsible for holding the 16-step pattern for one track and
providing methods to modify and query that pattern.
*/

#pragma once

#include <vector>
#include <iostream>
#include <algorithm> // For std::fill

#include <SFML/Audio.hpp>

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

class SeqTrack
{
public:
    SeqTrack(int trackIndex, int numSteps);
    void toggleStep(int step);
    bool isStepActive(int step);
    void setSample(SoundBuffer& sampleBuffer);
    void trigger();
    void clear();

private:
    int trackIndex;
    int numSteps;
    vector<bool> steps; // 16 steps, true = on, false = off
    Sound sample;
};
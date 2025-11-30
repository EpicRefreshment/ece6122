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
#include <algorithm>
#include <random>
#include <cstdlib>

#include <SFML/Audio.hpp>

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

class SeqTrack
{
public:
    SeqTrack(int trackIndex, int numSteps);
    void setSample(SoundBuffer& sampleBuffer);

    void trigger();
    void processTick(long long globalTick, bool anyTrackIsSoloed);
    void clear();
    void reset();

    bool isStepActive(int step);
    bool muted();
    bool soloed();

    void setMode(int dropdownIndex);
    int getMode() const;

    int getCurrentStep() const;
    int getTrackLength() const;
    double getTempoDivision() const;
    int getProbability() const;
    int getRatchet() const;
    int getFill() const;
    int getShift() const;
    int getRule() const;
    int getInject() const;
    int getLock() const;
    int getXORScrambler() const;
    int getAdd() const;
    int getBitCount() const;

    void updateTrackLength(int direction);
    void updateTempoDivision(int direction);
    void updateProbability(int direction);
    void updateRatchet(int direction);
    void updateFill(int direction);
    void updateShift(int direction);
    void updateRule(int direction);
    void updateInject(int direction);
    void updateLock(int direction);
    void updateXORScrambler(int direction);
    void updateAdd(int direction);
    void updateBitCount(int direction);

    void toggleStep(int step);
    void toggleMute();
    void toggleSolo();

    void generate(int clicked, int modeChange);
    void setRegenRate(int direction);
    int getRegenRate();

    int getNumSteps() const;

private:
    void generateProbabilistic();
    void generateEuclidean();
    void generateCellularAutomata();
    void generateShiftRegister();
    void generateBinaryCounter();

    void shiftPattern(int shift);

    int trackIndex;
    int numSteps;
    vector<bool> steps; // 16 steps, true = on, false = off
    Sound sample;

    int currentStep;
    int mode;
    bool mute;
    bool solo;

    int trackLength;
    double tempoDivision; // e.g., 1.0 for normal, 2.0 for double-time, 0.5 for half-time
    int probability;
    int ratchet;
    int fill;
    int shift;
    uint8_t rule;
    int inject;
    int lock;
    int xorScrambler;
    int addAmount;
    int bitCount;

    vector<uint8_t> ruleList = {30, 54, 90, 102, 110, 122, 138, 154, 162, 170, 178, 186, 194, 202, 210};

    int regenRate;
    int regenTicks;

    int ticksPerStep;
};
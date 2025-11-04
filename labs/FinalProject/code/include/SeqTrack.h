/*
Author: [Your Name/Gemini]
Class: ECE6122Q
Date Created: 11/03/2025
Date Last Modified: 11/04/2025

Description:

Multimode Sequencer Project

This is the header file for the SeqTrack class.
This class defines the properties and methods for a single sequencer track.
It is responsible for holding the 16-step pattern for one track and
providing methods to modify and query that pattern.
*/

#ifndef SEQTRACK_H
#define SEQTRACK_H

#include <vector>

class SeqTrack
{
public:
    SeqTrack();
    void toggleStep(int step);
    bool isStepActive(int step) const;
    void trigger(int step, int trackIndex);
    void clear();

private:
    std::vector<bool> steps; // 16 steps, true = on, false = off
};

#endif // SEQTRACK_H
/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/15/2025
Date Last Modified: 11/04/2025

Description:

Final Project

This is the source file for the SeqTrack class.
It implements all functions defined in SeqTrack.h.
This class manages the 16-step state for one sequencer track.
*/

#include "SeqTrack.h"

using namespace std;

/*
This is the constructor for SeqTrack.
It initializes the 16-step pattern to all 'off'.

Arguments:
    N/A
Return Values:
    SeqTrack
*/
SeqTrack::SeqTrack(int trackIndex, int numSteps)
{
    this->trackIndex = trackIndex;
    this->numSteps = numSteps;
    // Initialize the vector with 16 steps, all set to false (off)
    this->steps.resize(numSteps, false);

}

/*
Toggles the state of a single step (on/off).

Arguments:
    step - The step index (0-15) to toggle.
Return Values:
    void
*/
void SeqTrack::toggleStep(int step)
{
    if (step >= 0 && step < numSteps)
    {
        this->steps[step] = !this->steps[step];
    }
}

/*
Checks if a specific step is currently active (on).

Arguments:
    step - The step index (0-15) to check.
Return Values:
    bool - true if the step is active, false otherwise.
*/
bool SeqTrack::isStepActive(int step)
{
    if (step >= 0 && step < numSteps)
    {
        return this->steps[step];
    }
    return false;
}

void SeqTrack::setSample(SoundBuffer& sampleBuffer)
{
    sample.setBuffer(sampleBuffer);
}

/*
Triggers the step, "playing" it if it's active.
For now, this prints to the console.

Arguments:
    step - The current step index (0-15) being triggered.
    trackIndex - The index of this track (for console output).
Return Values:
    void
*/
void SeqTrack::trigger()
{
    sample.play();
}

/*
Clears all steps in the track (sets all to false).

Arguments:
    N/A
Return Values:
    void
*/
void SeqTrack::clear()
{
    std::fill(this->steps.begin(), this->steps.end(), false);
}
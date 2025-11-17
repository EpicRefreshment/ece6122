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
    steps.resize(numSteps, false);
    trackLength = numSteps;
    tempoDivision = 1.0;
    probability = 100;
    ticksPerStep = 16;
    currentStep = 0;
    mute = false;
    solo = false;
    mode = 0;
    srand(static_cast<unsigned int>(time(0)) + trackIndex); // Seed random generator
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
    if (step >= 0 && step < trackLength)
    {
        steps[step] = !steps[step];
    }
}

void SeqTrack::setSample(SoundBuffer& sampleBuffer)
{
    sample.setBuffer(sampleBuffer);
}

/*
Triggers the step, "playing" it if it's active.

Arguments:
    N/A
Return Values:
    void
*/
void SeqTrack::trigger()
{
    // The probability check is now part of the trigger logic
    if ((rand() % 101) <= probability)
    {
        sample.play();
    }
}

void SeqTrack::processTick(long long globalTick)
{
    // This track's step only advances when the global tick aligns with its tempo division
    if (globalTick % ticksPerStep == 0)
    {
        if (steps[currentStep] && !mute)
        {
            if ((rand() % 101) <= probability)
                sample.play();
        }
        currentStep = (currentStep + 1) % trackLength;
    }
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
    fill(this->steps.begin(), this->steps.end(), false);
}

void SeqTrack::reset()
{
    currentStep = 0;
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

bool SeqTrack::muted()
{
    return mute;
}

bool SeqTrack::soloed()
{
    return solo;
}

int SeqTrack::getCurrentStep() const
{
    return currentStep;
}

int SeqTrack::getTrackLength() const
{
    return trackLength;
}

double SeqTrack::getTempoDivision() const
{
    return tempoDivision;
}

int SeqTrack::getProbability() const
{
    return probability;
}

void SeqTrack::updateParam1(int direction)
{
    switch (mode)
    {
        case 0:
            // Step Sequencer
            if (direction && trackLength < numSteps)
            {
                trackLength++;
            }
            else if (!direction && trackLength > 0)
            {
                trackLength--;
            }
            break;
        default:
            break;
    }
}

void SeqTrack::updateParam2(int direction)
{
    switch (mode)
    {
        case 0:
        {
            // Step Sequencer
            double currentDiv = tempoDivision;
            if (direction && currentDiv < 8.0)
            {
                tempoDivision = currentDiv * 2.0;
            }
            else if (!direction && currentDiv > (1.0 / 8.0))
            {
                tempoDivision = currentDiv / 2.0;
            }
            // This calculation must be atomic with the change
            ticksPerStep = static_cast<int>(16.0 / tempoDivision);
            break;
        }
        default:
            break;
    }
}

void SeqTrack::updateParam3(int direction)
{
    switch (mode)
    {
        case 0:
            // Step Sequencer
            if (direction && probability < 100)
            {
                probability += 5;
            }
            else if (!direction && probability > 0)
            {
                probability -= 5;
            }
            break;
        default:
            break;
    }
}

void SeqTrack::toggleMute()
{
    mute = !mute;
}

void SeqTrack::toggleSolo()
{
    solo = !solo;
}

int SeqTrack::getNumSteps() const
{
    return numSteps;
}
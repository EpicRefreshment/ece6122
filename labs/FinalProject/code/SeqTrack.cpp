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

    mode = 0;
    trackLength = numSteps;
    tempoDivision = 1.0;
    probability = 100;
    ratchet = 1;

    ticksPerStep = 16;
    currentStep = 0;
    mute = false;
    solo = false;

    regenRate = 0;
    regenTicks = 0;

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

void SeqTrack::processTick(long long globalTick, bool anyTrackIsSoloed)
{
    // This track's step only advances when the global tick aligns with its tempo division
    if (globalTick % ticksPerStep == 0)
    {
        bool trackActive = false;
        if (anyTrackIsSoloed)
        {
            // If any track is soloed, only play this track if it is also soloed (and not muted).
            trackActive = solo && !mute;
        }
        else
        {
            // If no tracks are soloed, play this track as long as it's not muted.
            trackActive = !mute;
        }

        if (trackActive && steps[currentStep])
        {
            trigger();
        }

        currentStep = (currentStep + 1) % trackLength;
        if (currentStep == 0 && regenRate > 0)
        {
            generate(0, 0);
        }
    }
    else if (ratchet > 1 && globalTick % (ticksPerStep / ratchet) == 0)
    {
        // Calculate the sub-step within the current step
        int prevStepActive = 0;
        if (currentStep == 0 && steps[trackLength - 1])
        {
            prevStepActive = 1;
        }
        else if (currentStep != 0 && steps[currentStep - 1])
        {
            prevStepActive = 1;
        }

        // 50% chance a ratchet step occurs. Increases the randomness!
        if (prevStepActive &&(rand() % 101) <= 50)
        {
            trigger();
        }
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

void SeqTrack::setMode(int dropdownIndex)
{
    mode = dropdownIndex;
}

int SeqTrack::getMode() const
{
    return mode;
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

int SeqTrack::getRatchet() const
{
    return ratchet;
}

void SeqTrack::updateTrackLength(int direction)
{
    if (direction && trackLength < numSteps)
    {
        trackLength++;
    }
    else if (!direction && trackLength > 0)
    {
        trackLength--;
    }
}

void SeqTrack::updateTempoDivision(int direction)
{
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
}

void SeqTrack::updateProbability(int direction)
{
    if (direction && probability < 100)
    {
        probability += 5;
    }
    else if (!direction && probability > 0)
    {
        probability -= 5;
    }
}

void SeqTrack::updateRatchet(int direction)
{
    if (direction && ratchet < 4)
    {
        ratchet++;
    }
    else if (!direction && ratchet > 1)
    {
        ratchet--;
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

void SeqTrack::generate(int clicked, int modeChange)
{
    if (modeChange)
    {
        for (int i = 0; i < trackLength; i++)
        {
            // default to 50 here since density isn't a parameter
            // when the modeChange is 1
            if ((rand() % 101) <= 50)
            {
                steps[i] = true;
            }
            else
            {
                steps[i] = false;
            }
        }
        return;
    }

    if (!clicked)
    {
        regenTicks++;
        if (regenTicks == regenRate)
        {
            regenTicks = 0;
        }
        else
        {
            return;
        }
    }

    switch (mode)
    {
        case 0: // Probabilistic Step Sequencer
        {
            for (int i = 0; i < trackLength; i++)
            {
                if ((rand() % 101) <= 65)
                {
                    steps[i] = true;
                }
                else
                {
                    steps[i] = false;
                }
            }
            break;
        }
        case 1: // Euclidean Sequencer
        {
            break;
        }
        case 2: // Cellular Automata Sequencer
        {
            break;
        }
        case 3: // Shift Register Sequencer
        {
            break;
        }
        case 4: // Markov Chain Sequencer
        {
            break;
        }
    }
}

void SeqTrack::setRegenRate(int direction)
{
    if (direction && regenRate < 8)
    {
        regenRate++;
    }
    else if (!direction && regenRate > 0)
    {
        regenRate--;
    }
}

int SeqTrack::getRegenRate()
{
    return regenRate;
}

int SeqTrack::getNumSteps() const
{
    return numSteps;
}
/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/15/2025
Date Last Modified: 11/04/2025

Description:

Final Project

This is the source file for the SequencerEngine class.
It implements all functions defined in SequencerEngine.h.
This class handles the core timing and transport for the sequencer.
It does NOT hold any step data, only the clock and play state.
*/

#include "SequencerEngine.h"

using namespace std;
using namespace sf;

/*
This is the constructor for SequencerEngine.
Initializes transport variables and sets the default BPM.

Arguments:
    N/A
Return Values:
    SequencerEngine
*/
SequencerEngine::SequencerEngine()
{
    playing = 0;
    paused = 0;
    stopped = 1;

    currentStep = -1;

    bpm = 120;
    stepTime = sf::seconds((60.0f / bpm) / 4.0f);
}

/*
Starts the sequencer playback from the beginning.
Restarts the internal clock.

Arguments:
    N/A
Return Values:
    void
*/
void SequencerEngine::play()
{
    playing = 1;
    paused = 0;
    stopped = 0;
    clock.restart(); // Always play from the start
}

/*
Stops the sequencer playback.

Arguments:
    N/A
Return Values:
    void
*/
void SequencerEngine::stop()
{
    // reset playhead
    playing = 0;
    paused = 0;
    stopped = 1;
    currentStep = -1;
}

/*
Pauses the sequencer playback. (Currently same as stop).

Arguments:
    N/A
Return Values:
    void
*/
void SequencerEngine::pause()
{
    // stop playhead at current position
    playing = 0;
    paused = 1;
    stopped = 0;
}

/*
Updates the engine's internal clock.
Checks if enough time has passed to signal a 16th note tick.

Arguments:
    N/A
Return Values:
    bool - true if a new 16th note tick has occurred, false otherwise.
*/
bool SequencerEngine::update()
{
    if (!playing)
    {
        return false;
    }

    // Check if enough time has passed to signal a tick
    if (clock.getElapsedTime() >= stepTime)
    {
        currentStep = (currentStep + 1) % 16; // Advance step 
        clock.restart(); // Reset the clock for the *next* tick
        return true; // Signify that a tick has occurred
    }

    return false; // No tick this frame
}

/*
Sets the BPM and recalculates the stepTime.

Arguments:
    newBpm - The new beats-per-minute value.
Return Values:
    void
*/
void SequencerEngine::setBpm(int bpm)
{
    this->bpm = bpm;
    // (60 seconds / BPM) = duration of one beat (quarter note).
    // We have 4 steps per beat (16th notes).
    stepTime = sf::seconds((60.0f / bpm) / 4.0f);
}

int SequencerEngine::getBPM()
{
    return bpm;
}

/*
Checks if the sequencer is currently playing.

Arguments:
    N/A
Return Values:
    bool - true if playing, false if stopped/paused.
*/
int SequencerEngine::isPlaying()
{
    return playing;
}

int SequencerEngine::isPaused()
{
    return paused;
}

int SequencerEngine::isStopped()
{
    return stopped;
}

/*
Gets the current active step index.

Arguments:
    N/A
Return Values:
    int - The current step (0-15), or -1 if stopped.
*/
int SequencerEngine::getCurrentStep()
{
    return currentStep;
}
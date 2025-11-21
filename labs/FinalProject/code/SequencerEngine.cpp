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
SequencerEngine::SequencerEngine(const vector<SeqTrack*>& tracks, ThreadPool& pool)
    : tracks(tracks), pool(pool)
{
    playing = 0;
    paused = 0;
    stopped = 1;

    globalStep = -1;
    globalTick = 0;

    processingTime = 0;
    timingDeviation = 0;
    lastTickTime = 0;
    lastTickScheduledTime = 0;

    setBpm(120);

    seqThread = thread(&SequencerEngine::run, this);

    // Start performance measurement in a separate thread
    pool.enqueueTask([this]
    {
        this->measurePerformance();
    });
}

SequencerEngine::~SequencerEngine()
{
    stopFlag = true;
    if (seqThread.joinable())
    {
        seqThread.join();
    }
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
    postCommand([this]() {
        playing = true;
        paused = false;
        stopped = false;
        elapsedTime = sf::Time::Zero;
        clock.restart(); // Always play from the start
        globalTick = 0;
        globalStep = 0; // Ensure visual step is also reset
    });
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
    postCommand([this]() {
        // reset playhead
        playing = false;
        paused = false;
        stopped = true;
        globalStep = -1;
        globalTick = -1; // Set to -1 so the next play() starts on tick 0
        ticked = true; // Force a UI update to hide the playhead

        // Also reset the individual playhead of each track
        for (auto* track : tracks)
        {
            track->reset();
        }
    });
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
    postCommand([this]() {
        // stop playhead at current position
        playing = false;
        paused = true;
        stopped = false;
    });
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
    // Atomically check if a tick has occurred and reset the flag.
    // This is a safe, non-blocking way for the UI thread to query the audio thread's state.
    bool ticked = this->ticked.exchange(false);
    return ticked;
}

/*
Updates the engine's internal clock.
Checks if enough time has passed to signal a 16th note tick.

Arguments:
    N/A
Return Values:
    N/A
*/
void SequencerEngine::run()
{
    auto processCommands = [this]() {
        function<void()> command;
        while (commandQueue.try_pop(command))
        {
            command();
        }
    };

    while (!stopFlag)
    {
        processCommands();

        bool isPlaying = playing.load();
        if (!isPlaying)
        {
            // Not playing, so sleep for a bit to yield CPU and restart the clock
            // to prevent accumulating a huge elapsedTime.
            clock.restart();
            this_thread::sleep_for(chrono::milliseconds(10));

            // Reset scheduled time when not playing
            lastTickScheduledTime.store(perfClock.getElapsedTime().asMicroseconds());

            continue;
        }
        
        elapsedTime += clock.restart();

        // Use a while loop to process all pending ticks. This is the core timing logic.
        while (elapsedTime >= stepTime)
        {
            // Process commands before each tick to handle stop/pause immediately.
            processCommands();
            if (!playing.load()) {
                break; // Exit tick processing if a command stopped playback.
            }

            long long scheduledTime = lastTickScheduledTime.load();
            lastTickScheduledTime.store(scheduledTime + stepTime.asMicroseconds());

            elapsedTime -= stepTime;
            long long currentTick = globalTick.fetch_add(1);
            lastTickTime.store(perfClock.getElapsedTime().asMicroseconds());

            globalStep = (currentTick / 16) % 16;

            // For each track, check if it should be triggered on this tick.
            // First, determine if any track is in solo mode.
            bool anyTrackIsSoloed = false;
            for (const auto* track : tracks)
            {
                if (const_cast<SeqTrack*>(track)->soloed())
                {
                    anyTrackIsSoloed = true;
                    break;
                }
            }

            for (auto* track : tracks)
            {
                pool.enqueueTask([track, currentTick, anyTrackIsSoloed] {
                    track->processTick(currentTick, anyTrackIsSoloed);
                });
            }
            
            ticked = true; // Signal to the UI thread that a tick happened.
        }
        // Sleep for a very short duration to be responsive but not burn 100% CPU.
        this_thread::sleep_for(chrono::microseconds(500));
    }
}

void SequencerEngine::measurePerformance()
{
    long long lastMeasuredTick = 0;
    long long processingStartTime = 0;

    while (!stopFlag)
    {
        long long currentTick = globalTick.load();
        if (currentTick > lastMeasuredTick)
        {
            if (currentTick % 16 == 1) // Start of a 16-tick block
            {
                processingStartTime = lastTickTime.load();
            }

            if (currentTick % 16 == 0 && currentTick > 0) // End of a 16-tick block
            {
                long long processingEndTime = lastTickTime.load();
                processingTime.store(processingEndTime - processingStartTime);

                long long scheduledTime = lastTickScheduledTime.load();
                long long actualTime = lastTickTime.load();
                timingDeviation.store(actualTime - scheduledTime);

                cout << "Processing time for last 16 ticks: " << processingTime.load() << " us. "
                     << "Timing deviation: " << timingDeviation.load() << " us." << endl;
            }

            lastMeasuredTick = currentTick;
        }

        // Sleep to avoid busy-waiting
        this_thread::sleep_for(chrono::milliseconds(1));
    }
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
    this->bpm.store(bpm);
    // (60 seconds / BPM) = duration of one beat (quarter note).
    // 4 steps per beat (16th notes).
    // 16 ticks per step.
    stepTime = sf::seconds(((60.0f / bpm) / 4.0f) / 16.0f);
    lastTickScheduledTime.store(perfClock.getElapsedTime().asMicroseconds());
}

void SequencerEngine::postCommand(function<void()> command)
{
    commandQueue.push(move(command));
}

int SequencerEngine::getBPM()
{
    return bpm.load();
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
    return playing.load();
}

int SequencerEngine::isPaused()
{
    return paused.load();
}

int SequencerEngine::isStopped()
{
    return stopped.load();
}

/*
Gets the current active step index.

Arguments:
    N/A
Return Values:
    int - The current step (0-15), or -1 if stopped.
*/
int SequencerEngine::getGlobalStep()
{
    return globalStep.load();
}
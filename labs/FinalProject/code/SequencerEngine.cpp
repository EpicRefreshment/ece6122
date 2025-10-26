#include "SequencerEngine.h"
#include <iostream>

using namespace std;

SequencerEngine::SequencerEngine()
{
    playing = false;
    currentStep = -1;
    bpm = 120.0f;
    grid = vector<bool>(16 * 8, false);
    // Calculate the time for each 16th note step based on the BPM.
    // (60 seconds / BPM) = duration of one beat.
    // We have 4 steps per beat (16th notes).
    stepTime = sf::seconds(60.0f / bpm / 4.0f);
}

void SequencerEngine::play() {
    playing = true;
    currentStep = -1; // Set to -1 so the first step is 0
    clock.restart();
}

void SequencerEngine::stop() {
    playing = false;
    currentStep = -1;
}

void SequencerEngine::update() {
    if (!playing) {
        return;
    }

    // Check if enough time has passed to advance to the next step
    if (clock.getElapsedTime() >= stepTime) {
        currentStep = (currentStep + 1) % 16;
        clock.restart(); // Reset the clock for the next step

        // If the current step is active, "play a note"
        // For now, this just prints to the console.
        // This is where you would send a MIDI message.
        if (grid[currentStep]) {
            std::cout << "Note On: Step " << currentStep << std::endl;
        }
    }
}

void SequencerEngine::toggleStep(int step) {
    if (step >= 0 && step < 16) {
        grid[step] = !grid[step];
    }
}

// --- Getter Implementations ---

bool SequencerEngine::isPlaying() const {
    return playing;
}

int SequencerEngine::getCurrentStep() const {
    return currentStep;
}

const std::vector<bool>& SequencerEngine::getGridState() const {
    return grid;
}
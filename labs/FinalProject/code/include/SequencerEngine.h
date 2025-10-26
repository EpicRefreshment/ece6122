#pragma once

#include <SFML/System/Clock.hpp>
#include <vector>

class SequencerEngine {
public:
    // Constructor: Initializes the sequencer with a default state.
    SequencerEngine();

    // Starts or stops the playback.
    void play();
    void stop();

    // The main update function, called on every frame to advance the sequence.
    void update();

    // Toggles the state of a specific step in the sequence (on/off).
    void toggleStep(int step);

    // --- Getters for the GUI ---
    bool isPlaying() const;
    int getCurrentStep() const;
    const std::vector<bool>& getGridState() const;

private:
    bool playing;
    int currentStep;
    float bpm;
    sf::Time stepTime;
    sf::Clock clock;
    std::vector<bool> grid; // 16 steps for the sequence
};
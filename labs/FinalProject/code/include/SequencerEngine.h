/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/15/2025
Date Last Modified: 11/04/2025

Description:

Final Project

This is the header file for the SequencerEngine class.
This class manages the core timing and transport controls (play, stop, pause).
It functions as a metronome, providing a "tick" via its update() method
based on the BPM. It does not manage step state.
*/

#pragma once

#include <thread>
#include <mutex>
#include <atomic>

#include <vector>
#include "SeqTrack.h"
#include "ThreadPool.h"
#include "ThreadSafeQueue.h"

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

// Make code easier to type with "using namespace"
using namespace std;
using namespace sf;

class SequencerEngine
{
public:
    SequencerEngine(const vector<SeqTrack*>& tracks, ThreadPool& pool);
    ~SequencerEngine();

    void play();
    void stop();
    void pause();
    bool update();
    void run();

    int isPlaying();
    int isPaused();
    int isStopped();

    void setBpm(int bpm);

    // A thread-safe way for the UI to post commands to the engine
    void postCommand(function<void()> command);

    int getBPM();
    int getGlobalStep();
private:
    atomic<bool> playing;
    atomic<bool> paused;
    atomic<bool> stopped;
    atomic<bool> stopFlag;

    atomic<int> bpm;
    atomic<int> globalStep;
    atomic<long long> globalTick;

    atomic<bool> ticked;

    Clock clock;
    Time elapsedTime; // Accumulates time for drift-free timing
    Time stepTime; // Duration of one tick

    thread seqThread;
    const vector<SeqTrack*>& tracks;
    ThreadPool& pool;
    ThreadSafeQueue<function<void()>> commandQueue;
};
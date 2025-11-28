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
    fill = 8;
    shift = 0;

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
        if (currentStep == 0 && regenRate > 0 && mode != 1)
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
    std::fill(this->steps.begin(), this->steps.end(), false);
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
    switch (mode)
    {
        case 0: // Probabilistic Step Sequencer
            trackLength = numSteps;
            tempoDivision = 1.0;
            probability = 100;
            ratchet = 1;
            break;
        case 1: // Euclidean Sequencer
            trackLength = numSteps;
            tempoDivision = 1.0;
            probability = 100; // set so this parameter essentially inactive since always in use
            ratchet = 1; // set so this parameter essentially inactive since always in use
            regenRate = 0;
            fill = 8;
            shift = 0;
            break;
        case 2: // Cellular Automata Sequencer
            trackLength = numSteps;
            tempoDivision = 1.0;
            probability = 100; // set so this parameter essentially inactive since always in use
            ratchet = 1; // set so this parameter essentially inactive since always in use
            regenRate = 0;
            rule = ruleList[0];
            inject = 0;
            break;
        case 3: // Shift Register Sequencer
            trackLength = numSteps;
            tempoDivision = 1.0;
            probability = 100; // set so this parameter essentially inactive since always in use
            ratchet = 1; // set so this parameter essentially inactive since always in use
            regenRate = 0;
            xorScrambler = 0;
            inject = 0;
            break;
        case 4: // Binary Counter Sequencer
            trackLength = numSteps;
            tempoDivision = 1.0;
            probability = 100; // set so this parameter essentially inactive since always in use
            ratchet = 1; // set so this parameter essentially inactive since always in use
            regenRate = 0;
            addAmount = 1;
            endianness = 0;
            break;
    }
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

int SeqTrack::getFill() const
{
    return fill;
}

int SeqTrack::getShift() const
{
    return shift;
}

int SeqTrack::getRule() const
{
    return rule;
}

int SeqTrack::getInject() const
{
    return inject;
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
        for (int i = trackLength; i < numSteps; i++)
        {
            steps[i] = false;
        }
    }
    if (fill > trackLength)
    {
        fill = trackLength;
    }
    if (shift > trackLength)
    {
        shift = trackLength;
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

void SeqTrack::updateFill(int direction)
{
    if (direction && fill < trackLength)
    {
        fill++;
    }
    else if (!direction && fill > 0)
    {
        fill--;
    }
}

void SeqTrack::updateShift(int direction)
{
    if (direction && shift < trackLength - 1) {
        shift++;
    }
    else if (!direction && shift > 0) {
        shift--;
    }
}

void SeqTrack::updateRule(int direction)
{
    auto it = std::find(ruleList.begin(), ruleList.end(), rule);
    if (it != ruleList.end())
    {
        int index = static_cast<int>(std::distance(ruleList.begin(), it));
        if (direction && index < static_cast<int>(ruleList.size()) - 1)
        {
            rule = ruleList[index + 1];
        }
        else if (!direction && index > 0)
        {
            rule = ruleList[index - 1];
        }
    }
}

void SeqTrack::updateInject(int direction)
{
    inject = !inject;
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
    if (modeChange && (mode == 0 or mode == 2))
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
        if (mode == 1)
        {
            return; // regenerate inactive in this mode
        }
        else if (regenTicks == regenRate) // continue on, reset ticks
        {
            regenTicks = 0;
        }
        else // back out of function
        {
            return;
        }
    }

    switch (mode)
    {
        case 0: // Probabilistic Step Sequencer
            generateProbabilistic();
            break;
        case 1: // Euclidean Sequencer
            generateEuclidean();
            break;
        case 2: // Cellular Automata Sequencer
            generateCellularAutomata();
            break;
        case 3: // Shift Register Sequencer
            break;
        case 4: // Logic Sequencer
            break;
    }
}

void SeqTrack::generateProbabilistic()
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
}

void SeqTrack::generateEuclidean()
{
    vector<vector<bool>> patterns;
    if (fill > trackLength)
    {
        return;
    }

    // Generate initial pattern as column, when patterns is just one vector we're done.
    // Values up to fill value are True, then up to track length are False
    for (int i = 0; i < trackLength; i++)
    {
        if (i < fill)
        {
            patterns.push_back({true});
        }
        else
        {
            patterns.push_back({false});
        }
    }

    cout << "Initial Pattern: " << endl;
    for (size_t x = 0; x < patterns.size(); x++)
    {
        for (size_t y = 0; y < patterns[x].size(); y++)
        {
            cout << patterns[x][y] << " ";
        }
        cout << endl;
    }

    // Generate a sequence of values of size {fill}, t, that
    // tell the algorithm how many "rows" to distribute
    vector<int> t(trackLength, 0);
    int quotient = trackLength / fill;
    int remainder = trackLength % fill;
    int startIndex = 0; 
    // remove {fill} # of columns until less than {fill} silences remain
    for (int i = 0; i < (quotient - 1); i++) 
    {
        t[i] = fill;
        startIndex++;
    }
    // remove {remainder} # of columns
    for (int i = startIndex; i < trackLength; i++)
    {
        if (remainder == 0)
        {
            break;
        }
        t[i] = remainder;

        if (i != 0)
        {
            quotient = t[i - 1] / t[i];
            remainder = t[i - 1] % t[i];
        }
        else
        {
            quotient = fill / t[i];
            remainder = fill % t[i];
        }

        if (quotient > 1)
        {
            i++;
            t[i] = t[i-1];
        }
    }

    cout << "t: ";
    for (size_t x = 0; x < t.size(); x++)
    {
        cout << t[x] << " ";
    }
    cout << endl;
            
    for (int i = 0; i < trackLength; i++)
    {
        if (patterns.size() == 1 || t[i] < 2)
        {
            break;
        }

        // pop back patterns and evenly distribute starting with first vector.
        for (int j = 0; j < t[i]; j++)
        {
            vector<bool> toMove = patterns.back();
            patterns[j].insert(patterns[j].end(), toMove.begin(), toMove.end());
            patterns.pop_back();
        }
        cout << "Remainder Pattern (t = " << t[i] << "): " << endl;
        for (size_t x = 0; x < patterns.size(); x++)
        {
            for (size_t y = 0; y < patterns[x].size(); y++)
            {
                cout << patterns[x][y] << " ";
            }
            cout << endl;
        }
    }
    // just pop back vector and add to previous vector until there's only one vector left.
    while (patterns.size() > 1)
    {
        int endIndex = static_cast<int>(patterns.size()) - 1;
        vector<bool> toMove = patterns.back();
        patterns[endIndex - 1].insert(patterns[endIndex - 1].end(), toMove.begin(), toMove.end());
        patterns.pop_back();
    }

    vector<bool> resultingPattern = patterns.back();

    for (int i = 0; i < trackLength; i++)
    {
        steps[i] = resultingPattern[i];
    }

    if (shift > 0)
    {
        shiftPattern(shift);
    }
    cout << "Shifted Pattern (shift = " << shift << "): " << endl;
    for (size_t x = 0; x < steps.size(); x++)
    {
        cout << steps[x] << " ";
    }
    cout << endl;
}

void SeqTrack::generateCellularAutomata()
{
    vector<bool> newPattern(trackLength, false);
    if (inject == 1) // inject a live cell on each side
    {
        steps[0] = true;
        steps[trackLength - 1] = true;
    }
    for (int i = 0; i < trackLength; i++)
    {
        // Get left, center, right cells with wrap-around
        bool left = steps[(i - 1 + trackLength) % trackLength];
        bool center = steps[i];
        bool right = steps[(i + 1) % trackLength];

        // Create a 3-bit number from the cell states
        int neighborhood = (left << 2) | (center << 1) | right;

        // Determine the new state based on the rule
        bool newState = (rule >> neighborhood) & 1;
        newPattern[i] = newState;
    }
    for (int i = 0; i < trackLength; i++)
    {
        steps[i] = newPattern[i];
    }
}

void SeqTrack::shiftPattern(int shift)
{
    // Perform shift
    vector<bool> rotatedPattern(trackLength, false);
    for (int i = 0; i < trackLength; i++)
    {
        int index = i - shift;
        if (index < 0)
        {
            index += trackLength;
        }
        rotatedPattern[i] = steps[index];
    }
    for (int i = 0; i < trackLength; i++)
    {
        steps[i] = rotatedPattern[i];
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
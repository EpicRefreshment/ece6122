/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/06/2025
Date Last Modified: 11/06/2025

Description:

Final Project

Source file for the TrackControlPanel class.
Implements all setup, event handling, and drawing for the
side panel used for managing individual track properties.
*/

#include "TrackControlPanel.h"
 
TrackControlPanel::TrackControlPanel(RenderWindow& window, SequencerEngine& engine, const Font& font, const vector<SeqTrack*>& tracks, Vector2f panelSize, Vector2f panelPos)
    : window(window), engine(engine), font(font), tracks(tracks)
{
    // Panel Background
    panelBackground.setSize(panelSize);
    panelBackground.setFillColor(Color(45, 45, 45));
    panelBackground.setPosition(panelPos);

    numTracks = static_cast<int>(tracks.size());

    activeDropdown = -1; // -1 means no dropdown is active

    // Dropdown menu background (initially invisible)
    dropdownBackground.setFillColor(Color(20, 20, 20));
    dropdownBackground.setOutlineColor(Color::White);
    dropdownBackground.setOutlineThickness(1.0f);

    initShapes();
    initDropdownItems();
}

void TrackControlPanel::handleMouse(Event event, float mousePosX, float mousePosY)
{
    // check dropdowns first
    this->handleDropdown(event, mousePosX, mousePosY);

    for (int i = 0; i < numTracks; i++)
    {
        // Get track mode just in case
        int trackMode = tracks[i]->getMode();

        // use all if branches and return to avoid iterating further if click location found.
        // Parameter 1 (Track Length) Up Button
        if (param1UpButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { 
                track->updateTrackLength(1); 
            });
            return;
        }
        // Parameter 1 (Track Length) Down Button
        if (param1DownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { 
                track->updateTrackLength(0); 
            });
            return;
        }
        // Parameter 2 (Tempo Division) Up Button
        if (param2UpButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { 
                track->updateTempoDivision(1); 
            });
            return;
        }
        // Parameter 2 (Tempo Division) Down Button
        if (param2DownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { 
                track->updateTempoDivision(0); 
            });
            return;
        }
        // Parameter 3 (Mode Dependent) Up Button
        if (param3UpButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            switch (trackMode)
            {
                case 0: // Probabilistic Step Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateProbability(1); 
                    });
                    break;
                case 1: // Euclidean Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateFill(1); 
                    });
                    break;
                case 2: // Cellular Automata Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateRule(1); 
                    });
                    break;
                case 3: // Shift Register Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateLock(1); 
                    });
                    break;
                case 4: // Binary Counter Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateAdd(1); 
                    });
                    break;
            }
            return;
        }
        // Parameter 3 (Mode Dependent) Down Button
        if (param3DownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            switch (trackMode)
            {
                case 0: // Probabilistic Step Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateProbability(0); 
                    });
                    break;
                case 1: // Euclidean Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateFill(0); 
                    });
                    break;
                case 2: // Cellular Automata Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateRule(0); 
                    });
                    break;
                case 3: // Shift Register Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateLock(0); 
                    });
                    break;
                case 4: // Binary Counter Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateAdd(0); 
                    });
                    break;
            }
            return;
        }
        // Parameter 4 (Mode Dependent) Up Button
        if (param4UpButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            switch (trackMode)
            {
                case 0: // Probabilistic Step Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateRatchet(1); 
                    });
                    break;
                case 1: // Euclidean Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateShift(1); 
                    });
                    break;
                case 2: // Cellular Automata Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateInject(1); 
                    });
                    break;
                case 3: // Shift Register Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateXORScrambler(1); 
                    });
                    break;
                case 4: // Binary Counter Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateBitCount(1); 
                    });
                    break;
            }
            return;
        }
        // Parameter 4 (Mode Dependent) Down Button
        if (param4DownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            switch (trackMode)
            {
                case 0: // Probabilistic Step Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateRatchet(0); 
                    });
                    break;
                case 1: // Euclidean Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateShift(0); 
                    });
                    break;
                case 2: // Cellular Automata Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateInject(0); 
                    });
                    break;
                case 3: // Shift Register Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateXORScrambler(0); 
                    });
                    break;
                case 4: // Binary Counter Sequencer
                    engine.postCommand([track = tracks[i]] { 
                        track->updateBitCount(0); 
                    });
                    break;
            }
            return;
        }
        // Mute Buttons
        if (muteButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] {
                track->toggleMute(); 
            });
            if (!tracks[i]->muted()) // Check the opposite state for immediate visual feedback
            {
                muteButtons[i].setOutlineColor(Color::Red);
                muteText[i].setFillColor(Color::Red);
            }
            else
            {
                muteButtons[i].setOutlineColor(Color(80, 80, 80));
                muteText[i].setFillColor(Color::White);
            }
            return;
        }
        // Solo Buttons
        if (soloButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] {
                track->toggleSolo();
            });
            if (!tracks[i]->soloed()) // Check the opposite state for immediate visual feedback
            {
                soloButtons[i].setOutlineColor(Color::Cyan);
                soloText[i].setFillColor(Color::Cyan);
            }
            else
            {
                soloButtons[i].setOutlineColor(Color(80, 80, 80));
                soloText[i].setFillColor(Color::White);
            }
            return;
        }
        // Generate Button
        if (generateButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] {
                track->generate(1, 0);
            });
            return;
        }
        // Regenerate Rate Up Button
        if (regenRateUpButtons[i].getGlobalBounds().contains(mousePosX, mousePosY) && trackMode != 1)
        {
            if (tracks[i]->getRegenRate() == 0)
            {
                generateButtons[i].setOutlineColor(Color::Green);
                generateText[i].setFillColor(Color::Green);
            }
            engine.postCommand([track = tracks[i]] {
                track->setRegenRate(1);
            });
            return;
        }
        if (regenRateDownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY) && trackMode != 1)
        {
            if (tracks[i]->getRegenRate() == 1)
            {
                generateButtons[i].setOutlineColor(Color(80, 80, 80));
                generateText[i].setFillColor(Color::White);
            }
            engine.postCommand([track = tracks[i]] {
                track->setRegenRate(0);
            });
            return;
        }
    }
}

void TrackControlPanel::handleDropdown(Event event, float mousePosX, float mousePosY)
{
    if (activeDropdown != -1)
    {
        // If a dropdown is open, check for clicks on its items
        for (size_t i = 0; i < dropdownItems.size(); i++)
        {
            if (dropdownItems[i].getGlobalBounds().contains(mousePosX, mousePosY))
            {
                modeText[activeDropdown].setString(dropdownItems[i].getString());
                tracks[activeDropdown]->setMode(static_cast<int>(i));
                engine.postCommand([track = tracks[activeDropdown]] {
                    track->generate(0, 1);
                });
                activeDropdown = -1; // Close dropdown
                return;
            }
        }

        // If clicked outside the dropdown, close it
        if (!dropdownBackground.getGlobalBounds().contains(mousePosX, mousePosY))
        {
            activeDropdown = -1;
        }
    }
    else
    {
        // If no dropdown is open, check for clicks on the main dropdown buttons
        for (int i = 0; i < numTracks; i++)
        {
            if (dropdownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
            {
                activeDropdown = i;
                return; // Stop after opening a dropdown
            }
        }
    }
}

void TrackControlPanel::update()
{
    // This update loop ensures the text always reflects the current state.
    for (int i = 0; i < numTracks; i++)
    {
        updateParam1Text(i);
        updateParam2Text(i);
        updateParam3Text(i);
        updateParam4Text(i);
        updateRegenText(i);
    }

    if (activeDropdown != -1)
    {
        const auto& button = dropdownButtons[activeDropdown];
        float dropdownHeight = dropdownItems.size() * 20.0f;
        float boundaryY = panelBackground.getPosition().y + panelBackground.getSize().y;
        dropdownBackground.setSize({button.getSize().x, dropdownHeight});
        if ((button.getPosition().y + button.getSize().y + dropdownHeight) < boundaryY)
        {
            dropdownBackground.setPosition(button.getPosition().x, button.getPosition().y + button.getSize().y);
        }
        else
        {
            dropdownBackground.setPosition(button.getPosition().x, button.getPosition().y -dropdownHeight);
        }

        for (size_t i = 0; i < dropdownItems.size(); i++)
        {
            dropdownItems[i].setPosition(dropdownBackground.getPosition().x + 5, dropdownBackground.getPosition().y + i * 20.0f);
        }
    }
}

void TrackControlPanel::draw()
{
    window.draw(panelBackground);

    for (const auto& bg : trackBackgrounds)
    {
        window.draw(bg);
    }

    for (int i = 0; i < numTracks; i++)
    {
        window.draw(trackLabels[i]);
        window.draw(dropdownButtons[i]);
        window.draw(modeText[i]);
        window.draw(param1Text[i]);
        window.draw(param1UpButtons[i]);
        window.draw(param1DownButtons[i]);
        window.draw(param2Text[i]);
        window.draw(param2UpButtons[i]);
        window.draw(param2DownButtons[i]);
        window.draw(param3Text[i]);
        window.draw(param3UpButtons[i]);
        window.draw(param3DownButtons[i]);
        window.draw(param4Text[i]);
        window.draw(param4UpButtons[i]);
        window.draw(param4DownButtons[i]);
        window.draw(param1UpText[i]);
        window.draw(param1DownText[i]);
        window.draw(param2UpText[i]);
        window.draw(param2DownText[i]);
        window.draw(param3UpText[i]);
        window.draw(param3DownText[i]);
        window.draw(param4UpText[i]);
        window.draw(param4DownText[i]);
        window.draw(muteButtons[i]);
        window.draw(soloButtons[i]);
        window.draw(muteText[i]);
        window.draw(soloText[i]);
        window.draw(generateButtons[i]);
        window.draw(generateText[i]);
        window.draw(regenRateText[i]);
        window.draw(regenRateUpButtons[i]);
        window.draw(regenRateDownButtons[i]);
        window.draw(regenRateUpText[i]);
        window.draw(regenRateDownText[i]);
    }
    // If a dropdown is active, draw it on top of everything else
    if (activeDropdown != -1)
    {
        window.draw(dropdownBackground);

        for (size_t i = 0; i < dropdownItems.size(); i++)
        {
            window.draw(dropdownItems[i]);
        }
    }
}

FloatRect TrackControlPanel::getGlobalBounds()
{
    return panelBackground.getGlobalBounds();
}

void TrackControlPanel::setupText(Text& text, const String& value, Vector2f position, int size)
{
    text.setFont(font);
    text.setString(value);
    text.setCharacterSize(size);
    text.setFillColor(Color::White);
    text.setPosition(position);
}

void TrackControlPanel::initShapes()
{

    resizeUIVectors();

    // Initialize all the GUI shapes
    float trackHeight = panelBackground.getSize().y / static_cast<float>(numTracks);

    for (int i = 0; i < numTracks; i++)
    {
        initTrackBackground(i, trackHeight);

        float trackY = panelBackground.getPosition().y + i * trackHeight;
        setupText(trackLabels[i], "Track " + to_string(i + 1), {panelBackground.getPosition().x + 5, trackY + 5}, 16);

        initDropdownButtons(i, trackHeight);
        initParameterControls(i, trackHeight);
        initMuteSoloControls(i, trackHeight);
        initGenerateControls(i, trackHeight);

        updateParam1Text(i);
        updateParam2Text(i);
        updateParam3Text(i);
        updateParam4Text(i);
        updateRegenText(i);
    }
}

void TrackControlPanel::resizeUIVectors()
{
    trackBackgrounds.resize(numTracks);
    trackLabels.resize(numTracks);
    dropdownButtons.resize(numTracks);
    modeText.resize(numTracks);

    param1Text.resize(numTracks);
    param1UpText.resize(numTracks);
    param1DownText.resize(numTracks);
    param1UpButtons.resize(numTracks);
    param1DownButtons.resize(numTracks);

    param2Text.resize(numTracks);
    param2UpText.resize(numTracks);
    param2DownText.resize(numTracks);
    param2UpButtons.resize(numTracks);
    param2DownButtons.resize(numTracks);

    param3Text.resize(numTracks);
    param3UpText.resize(numTracks);
    param3DownText.resize(numTracks);
    param3UpButtons.resize(numTracks);
    param3DownButtons.resize(numTracks);

    param4Text.resize(numTracks);
    param4UpText.resize(numTracks);
    param4DownText.resize(numTracks);
    param4UpButtons.resize(numTracks);
    param4DownButtons.resize(numTracks);

    generateText.resize(numTracks);
    generateButtons.resize(numTracks);

    regenRateText.resize(numTracks);
    regenRateUpText.resize(numTracks);
    regenRateDownText.resize(numTracks);
    regenRateUpButtons.resize(numTracks);
    regenRateDownButtons.resize(numTracks);

    muteButtons.resize(numTracks);
    soloButtons.resize(numTracks);
    muteText.resize(numTracks);
    soloText.resize(numTracks);
}

void TrackControlPanel::initTrackBackground(int index, float trackHeight)
{
    trackBackgrounds[index].setSize({panelBackground.getSize().x, trackHeight});
    trackBackgrounds[index].setPosition(panelBackground.getPosition().x, panelBackground.getPosition().y + index * trackHeight);
    trackBackgrounds[index].setOutlineColor(Color::Black);
    trackBackgrounds[index].setOutlineThickness(2.0f);

    // Shade every other row differently
    if (index % 2 == 1)
    {
            trackBackgrounds[index].setFillColor(Color(50, 50, 50));
    }
    else
    {
            trackBackgrounds[index].setFillColor(Color(40, 40, 40));
    }
}

void TrackControlPanel::initDropdownButtons(int index, float trackHeight)
{
    // Dropdown Button
    dropdownButtons[index].setSize({panelBackground.getSize().x - 10, trackHeight / 5.0f});
    dropdownButtons[index].setPosition(panelBackground.getPosition().x + 2, trackLabels[index].getPosition().y + 21);
    dropdownButtons[index].setFillColor(Color::Black);
    dropdownButtons[index].setOutlineColor(Color(80, 80, 80));
    dropdownButtons[index].setOutlineThickness(1.0f);

    // Mode Text (for dropdown buttons)
    setupText(modeText[index], "Probabilistic Step Sequencer", {dropdownButtons[index].getPosition().x + 5, dropdownButtons[index].getPosition().y + 2}, 14);
}

void TrackControlPanel::initParameterControls(int index, float trackHeight)
{
    // Parameter Text
    int paramTextSize = 16;
    float paramTextOffsetY = dropdownButtons[index].getPosition().y + dropdownButtons[index].getSize().y + 5;
    setupText(param1Text[index], "", {panelBackground.getPosition().x + 5, paramTextOffsetY}, paramTextSize);
    setupText(param2Text[index], "", {param1Text[index].getPosition().x + 100, paramTextOffsetY}, paramTextSize);
    setupText(param3Text[index], "", {param2Text[index].getPosition().x + 80, paramTextOffsetY}, paramTextSize);
    setupText(param4Text[index], "", {param3Text[index].getPosition().x + 100, paramTextOffsetY}, paramTextSize);

    // Parameter 1 Controls
    param1UpButtons[index].setSize({25, 20});
    param1UpButtons[index].setPosition({param1Text[index].getPosition().x, param1Text[index].getPosition().y + paramTextSize + 5});
    param1UpButtons[index].setFillColor(Color::Black);
    param1DownButtons[index].setSize({25, 20});
    param1DownButtons[index].setPosition({param1UpButtons[index].getPosition().x + param1UpButtons[index].getSize().x + 2, param1Text[index].getPosition().y + paramTextSize + 5});
    param1DownButtons[index].setFillColor(Color::Black);
    setupText(param1UpText[index], "+", {param1UpButtons[index].getPosition().x + 7, param1UpButtons[index].getPosition().y - 2}, 18);
    setupText(param1DownText[index], "-", {param1DownButtons[index].getPosition().x + 7, param1DownButtons[index].getPosition().y - 2}, 18);

    // Parameter 2 Controls
    param2UpButtons[index].setSize({25, 20});
    param2UpButtons[index].setPosition({param2Text[index].getPosition().x, param2Text[index].getPosition().y + paramTextSize + 5});
    param2UpButtons[index].setFillColor(Color::Black);
    param2DownButtons[index].setSize({25, 20});
    param2DownButtons[index].setPosition({param2UpButtons[index].getPosition().x + param2UpButtons[index].getSize().x + 2, param2Text[index].getPosition().y + paramTextSize + 5});
    param2DownButtons[index].setFillColor(Color::Black);
    setupText(param2UpText[index], "+", {param2UpButtons[index].getPosition().x + 7, param2UpButtons[index].getPosition().y - 2}, 18);
    setupText(param2DownText[index], "-", {param2DownButtons[index].getPosition().x + 7, param2DownButtons[index].getPosition().y - 2}, 18);

    // Parameter 3 Controls
    param3UpButtons[index].setSize({25, 20});
    param3UpButtons[index].setPosition({param3Text[index].getPosition().x, param3Text[index].getPosition().y + paramTextSize + 5});
    param3UpButtons[index].setFillColor(Color::Black);
    param3DownButtons[index].setSize({25, 20});
    param3DownButtons[index].setPosition({param3UpButtons[index].getPosition().x + param3UpButtons[index].getSize().x + 2, param3Text[index].getPosition().y + paramTextSize + 5});
    param3DownButtons[index].setFillColor(Color::Black);
    setupText(param3UpText[index], "+", {param3UpButtons[index].getPosition().x + 7, param3UpButtons[index].getPosition().y - 2}, 18);
    setupText(param3DownText[index], "-", {param3DownButtons[index].getPosition().x + 7, param3DownButtons[index].getPosition().y - 2}, 18);

    // Parameter 4 Controls
    param4UpButtons[index].setSize({25, 20});
    param4UpButtons[index].setPosition({param4Text[index].getPosition().x, param4Text[index].getPosition().y + paramTextSize + 5});
    param4UpButtons[index].setFillColor(Color::Black);
    param4DownButtons[index].setSize({25, 20});
    param4DownButtons[index].setPosition({param4UpButtons[index].getPosition().x + param4UpButtons[index].getSize().x + 2, param4Text[index].getPosition().y + paramTextSize + 5});
    param4DownButtons[index].setFillColor(Color::Black);
    setupText(param4UpText[index], "+", {param4UpButtons[index].getPosition().x + 7, param4UpButtons[index].getPosition().y - 2}, 18);
    setupText(param4DownText[index], "-", {param4DownButtons[index].getPosition().x + 7, param4DownButtons[index].getPosition().y - 2}, 18);
}

void TrackControlPanel::initMuteSoloControls(int index, float trackHeight)
{
    // Mute Buttons
    float muteOffsetX = panelBackground.getPosition().x + 5;
    float muteOffsetY = param1DownButtons[index].getPosition().y + param1DownButtons[index].getSize().y + 2;
    muteButtons[index].setSize({50, 20});
    muteButtons[index].setPosition({muteOffsetX, muteOffsetY});
    muteButtons[index].setFillColor(Color::Black);
    muteButtons[index].setOutlineColor(Color(80, 80, 80));
    muteButtons[index].setOutlineThickness(1.0f);
    setupText(muteText[index], "Mute", {muteButtons[index].getPosition().x + 5, muteButtons[index].getPosition().y + 2}, 14);

    // Solo Buttons
    float soloOffsetX = muteButtons[index].getPosition().x + muteButtons[index].getSize().x + 5;
    float soloOffsetY = muteButtons[index].getPosition().y;
    soloButtons[index].setSize({50, 20});
    soloButtons[index].setPosition({soloOffsetX, soloOffsetY});
    soloButtons[index].setFillColor(Color::Black);
    soloButtons[index].setOutlineColor(Color(80, 80, 80));
    soloButtons[index].setOutlineThickness(1.0f);
    setupText(soloText[index], "Solo", {soloButtons[index].getPosition().x + 5, soloButtons[index].getPosition().y + 2}, 14);
}

void TrackControlPanel::initGenerateControls(int index, float trackHeight)
{
    // Generate Buttons
    float genOffsetX = soloButtons[index].getPosition().x + soloButtons[index].getSize().x + 5;
    float genOffsetY = muteButtons[index].getPosition().y;
    generateButtons[index].setSize({80, 20});
    generateButtons[index].setPosition({genOffsetX, genOffsetY});
    generateButtons[index].setFillColor(Color::Black);
    generateButtons[index].setOutlineColor(Color(80, 80, 80));
    generateButtons[index].setOutlineThickness(1.0f);
    setupText(generateText[index], "Generate", {generateButtons[index].getPosition().x + 5, generateButtons[index].getPosition().y + 2}, 14);

    // Regenerate Rate Controls
    float regenOffsetX = generateButtons[index].getPosition().x + generateButtons[index].getSize().x + 10;
    float regenOffsetY = generateButtons[index].getPosition().y + 3;
    setupText(regenRateText[index], "Regen Rate: Off", {regenOffsetX, regenOffsetY + 2}, 12);

    regenRateUpButtons[index].setSize({25, 20});
    regenRateUpButtons[index].setPosition({regenRateText[index].getPosition().x + 130, regenRateText[index].getPosition().y});
    regenRateUpButtons[index].setFillColor(Color::Black);
    regenRateDownButtons[index].setSize({25, 20});
    regenRateDownButtons[index].setPosition({regenRateUpButtons[index].getPosition().x + regenRateUpButtons[index].getSize().x + 2, regenRateText[index].getPosition().y});
    regenRateDownButtons[index].setFillColor(Color::Black);
    setupText(regenRateUpText[index], "+", {regenRateUpButtons[index].getPosition().x + 7, regenRateUpButtons[index].getPosition().y - 2}, 18);
    setupText(regenRateDownText[index], "-", {regenRateDownButtons[index].getPosition().x + 7, regenRateDownButtons[index].getPosition().y - 2}, 18);
}

void TrackControlPanel::initDropdownItems()
{
    dropdownItems.resize(5);
    setupText(dropdownItems[0], "Probabilistic Step Sequencer", {0, 0}, 14);
    setupText(dropdownItems[1], "Euclidean Sequencer", {0, 0}, 14);
    setupText(dropdownItems[2], "Cellular Automata Sequencer", {0, 0}, 14);
    setupText(dropdownItems[3], "Shift Register Sequencer", {0, 0}, 14);
    setupText(dropdownItems[4], "Binary Counter Sequencer", {0, 0}, 14);
}

void TrackControlPanel::updateParam1Text(int trackIndex)
{
    param1Text[trackIndex].setString("Len: " + to_string(tracks[trackIndex]->getTrackLength()));
}

void TrackControlPanel::updateParam2Text(int trackIndex)
{
    double div = tracks[trackIndex]->getTempoDivision();
    string divStr;
    if (div >= 1.0)
    {
        divStr = to_string(static_cast<int>(div)) + "x";
    }
    else
    {
        divStr = "1/" + to_string(static_cast<int>(1.0 / div)) + "x";
    }
    param2Text[trackIndex].setString("Div: " + divStr);
}

void TrackControlPanel::updateParam3Text(int trackIndex)
{
    // Get track mode
    int trackMode = tracks[trackIndex]->getMode();
    switch (trackMode)
    {
        case 0: // Probabilistic Step Sequencer
            param3Text[trackIndex].setString("Prob: " + to_string(tracks[trackIndex]->getProbability()) + "%");
            break;
        case 1: // Euclidean Sequencer
            param3Text[trackIndex].setString("Fill: " + to_string(tracks[trackIndex]->getFill()));
            break;
        case 2: // Cellular Automata Sequencer
            param3Text[trackIndex].setString("Rule: " + to_string(tracks[trackIndex]->getRule()));
            break;
        case 3: // Shift Register Sequencer
            param3Text[trackIndex].setString("Lock: " + to_string(tracks[trackIndex]->getLock()) + "%");
            break;
        case 4: // Binary Counter Sequencer
            param3Text[trackIndex].setString("Add: " + to_string(tracks[trackIndex]->getAdd()));
            break;
    }
}

void TrackControlPanel::updateParam4Text(int trackIndex)
{
    // Get track mode
    int trackMode = tracks[trackIndex]->getMode();
    switch (trackMode)
    {
        case 0: // Probabilistic Step Sequencer
            param4Text[trackIndex].setString("PRpt: " + to_string(tracks[trackIndex]->getRatchet()));
            break;
        case 1: // Euclidean Sequencer
            param4Text[trackIndex].setString("Rot: " + to_string(tracks[trackIndex]->getShift()));
            break;
        case 2: // Cellular Automata Sequencer
            if (tracks[trackIndex]->getInject())
            {
                param4Text[trackIndex].setString("Injt: On");
            }
            else
            {
                param4Text[trackIndex].setString("Injt: Off");
            }
            break;
        case 3: // Shift Register Sequencer
            if (tracks[trackIndex]->getXORScrambler())  
            {
                param4Text[trackIndex].setString("XOR: On");
            }
            else
            {
                param4Text[trackIndex].setString("XOR: Off");
            }
            break;
        case 4: // Binary Counter Sequencer
            param4Text[trackIndex].setString("Bits: " + to_string(tracks[trackIndex]->getBitCount()));
            break;
    }
}

void TrackControlPanel::updateRegenText(int trackIndex)
{
    int regenRate = tracks[trackIndex]->getRegenRate();
    if (regenRate == 0)
    {
        regenRateText[trackIndex].setString("Regen Rate: Off");
    }
    else if (regenRate == 1)
    {
        regenRateText[trackIndex].setString("Regen Rate: 1 Bar");
    }
    else
    {
        regenRateText[trackIndex].setString("Regen Rate: " + to_string(tracks[trackIndex]->getRegenRate()) + " Bars");
    }
}
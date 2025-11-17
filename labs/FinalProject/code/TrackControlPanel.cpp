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

    for (int i = 0; i < numTracks; ++i)
    {
        if (param1UpButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            cout << "Param 1 Up" << endl;
            engine.postCommand([track = tracks[i]] { track->updateParam1(1); });
            return;
        }
        if (param1DownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            cout << "Param 1 Down" << endl;
            engine.postCommand([track = tracks[i]] { track->updateParam1(0); });
            return;
        }
        if (param2UpButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { track->updateParam2(1); });
            return;
        }
        if (param2DownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { track->updateParam2(0); });
            return;
        }
        if (param3UpButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { track->updateParam3(1); });
            return;
        }
        if (param3DownButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { track->updateParam3(0); });
            return;
        }
        if (muteButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { track->toggleMute(); });
            // The visual update will lag slightly, but the UI is responsive.
            // A more advanced solution would involve the engine posting state changes back to the UI.
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
        if (soloButtons[i].getGlobalBounds().contains(mousePosX, mousePosY))
        {
            engine.postCommand([track = tracks[i]] { track->toggleSolo(); });
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
    // This update loop now ensures the text always reflects the current state.
    for (int i = 0; i < numTracks; ++i)
    {
        updateTrackText(i);
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
    for (int i = 0; i < numTracks; ++i)
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
        window.draw(param1UpText[i]);
        window.draw(param1DownText[i]);
        window.draw(param2UpText[i]);
        window.draw(param2DownText[i]);
        window.draw(param3UpText[i]);
        window.draw(param3DownText[i]);
        window.draw(muteButtons[i]);
        window.draw(soloButtons[i]);
        window.draw(muteText[i]);
        window.draw(soloText[i]);
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
    float trackHeight = panelBackground.getSize().y / static_cast<float>(numTracks);
    float controlWidth = panelBackground.getSize().x - 10;

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

    muteButtons.resize(numTracks);
    soloButtons.resize(numTracks);
    muteText.resize(numTracks);
    soloText.resize(numTracks);

    // Initialize all the GUI shapes

    for (int i = 0; i < numTracks; ++i)
    {
        float trackY = panelBackground.getPosition().y + i * trackHeight;
        setupText(trackLabels[i], "Track " + to_string(i + 1), {panelBackground.getPosition().x + 5, trackY + 5}, 16);

        // Dropdown Button
        dropdownButtons[i].setSize({panelBackground.getSize().x - 10, trackHeight / 5.0f});
        dropdownButtons[i].setPosition(panelBackground.getPosition().x + 2, trackLabels[i].getPosition().y + 21);
        dropdownButtons[i].setFillColor(Color::Black);
        dropdownButtons[i].setOutlineColor(Color(80, 80, 80));
        dropdownButtons[i].setOutlineThickness(1.0f);

        // Mode Text (for dropdown buttons)
        setupText(modeText[i], "Step Sequencer", {dropdownButtons[i].getPosition().x + 5, dropdownButtons[i].getPosition().y + 2}, 14);

        // Parameter Text
        int paramTextSize = 16;
        float paramTextOffsetY = dropdownButtons[i].getPosition().y + dropdownButtons[i].getSize().y + 5;
        setupText(param1Text[i], "", {panelBackground.getPosition().x + 10, paramTextOffsetY}, paramTextSize);
        setupText(param2Text[i], "", {param1Text[i].getPosition().x + 80, paramTextOffsetY}, paramTextSize);
        setupText(param3Text[i], "", {param2Text[i].getPosition().x + 80, paramTextOffsetY}, paramTextSize);

        // Parameter 1 Controls
        param1UpButtons[i].setSize({25, 20});
        param1UpButtons[i].setPosition({param1Text[i].getPosition().x, param1Text[i].getPosition().y + paramTextSize + 5});
        param1UpButtons[i].setFillColor(Color::Black);
        param1DownButtons[i].setSize({25, 20});
        param1DownButtons[i].setPosition({param1UpButtons[i].getPosition().x + param1UpButtons[i].getSize().x + 2, param1Text[i].getPosition().y + paramTextSize + 5});
        param1DownButtons[i].setFillColor(Color::Black);
        setupText(param1UpText[i], "+", {param1UpButtons[i].getPosition().x + 7, param1UpButtons[i].getPosition().y - 2}, 18);
        setupText(param1DownText[i], "-", {param1DownButtons[i].getPosition().x + 7, param1DownButtons[i].getPosition().y - 2}, 18);

        // Parameter 2 Controls
        param2UpButtons[i].setSize({25, 20});
        param2UpButtons[i].setPosition({param2Text[i].getPosition().x, param2Text[i].getPosition().y + paramTextSize + 5});
        param2UpButtons[i].setFillColor(Color::Black);
        param2DownButtons[i].setSize({25, 20});
        param2DownButtons[i].setPosition({param2UpButtons[i].getPosition().x + param2UpButtons[i].getSize().x + 2, param2Text[i].getPosition().y + paramTextSize + 5});
        param2DownButtons[i].setFillColor(Color::Black);
        setupText(param2UpText[i], "+", {param2UpButtons[i].getPosition().x + 7, param2UpButtons[i].getPosition().y - 2}, 18);
        setupText(param2DownText[i], "-", {param2DownButtons[i].getPosition().x + 7, param2DownButtons[i].getPosition().y - 2}, 18);

        // Parameter 3 Controls
        param3UpButtons[i].setSize({25, 20});
        param3UpButtons[i].setPosition({param3Text[i].getPosition().x, param3Text[i].getPosition().y + paramTextSize + 5});
        param3UpButtons[i].setFillColor(Color::Black);
        param3DownButtons[i].setSize({25, 20});
        param3DownButtons[i].setPosition({param3UpButtons[i].getPosition().x + param3UpButtons[i].getSize().x + 2, param3Text[i].getPosition().y + paramTextSize + 5});
        param3DownButtons[i].setFillColor(Color::Black);
        setupText(param3UpText[i], "+", {param3UpButtons[i].getPosition().x + 7, param3UpButtons[i].getPosition().y - 2}, 18);
        setupText(param3DownText[i], "-", {param3DownButtons[i].getPosition().x + 7, param3DownButtons[i].getPosition().y - 2}, 18);

        // Mute Buttons
        float muteOffsetX = param3DownButtons[i].getPosition().x + param3DownButtons[i].getSize().x + 40;
        float muteOffsetY = param3DownButtons[i].getPosition().y + param3DownButtons[i].getSize().y + 2;
        muteButtons[i].setSize({50, 20});
        muteButtons[i].setPosition({muteOffsetX, muteOffsetY});
        muteButtons[i].setFillColor(Color::Black);
        muteButtons[i].setOutlineColor(Color(80, 80, 80));
        muteButtons[i].setOutlineThickness(1.0f);
        setupText(muteText[i], "Mute", {muteButtons[i].getPosition().x + 5, muteButtons[i].getPosition().y + 2}, 14);

        // Solo Buttons
        float soloOffsetX = muteButtons[i].getPosition().x + muteButtons[i].getSize().x + 5;
        float soloOffsetY = muteButtons[i].getPosition().y;
        soloButtons[i].setSize({50, 20});
        soloButtons[i].setPosition({soloOffsetX, soloOffsetY});
        soloButtons[i].setFillColor(Color::Black);
        soloButtons[i].setOutlineColor(Color(80, 80, 80));
        soloButtons[i].setOutlineThickness(1.0f);
        setupText(soloText[i], "solo", {soloButtons[i].getPosition().x + 5, soloButtons[i].getPosition().y + 2}, 14);

        updateTrackText(i);
    }
}

void TrackControlPanel::initDropdownItems()
{
    dropdownItems.resize(5);
    setupText(dropdownItems[0], "Step Sequencer", {0, 0}, 14);
    setupText(dropdownItems[1], "Stochastic Sequencer", {0, 0}, 14);
    setupText(dropdownItems[2], "Euclidean Sequencer", {0, 0}, 14);
    setupText(dropdownItems[3], "Shift Register Sequencer", {0, 0}, 14);
    setupText(dropdownItems[4], "Markov Chain Sequencer", {0, 0}, 14);
}

void TrackControlPanel::updateTrackText(int trackIndex)
{
    param1Text[trackIndex].setString("Len: " + to_string(tracks[trackIndex]->getTrackLength()));

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

    param3Text[trackIndex].setString("Prob: " + to_string(tracks[trackIndex]->getProbability()));
}

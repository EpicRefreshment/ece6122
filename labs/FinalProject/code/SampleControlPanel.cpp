/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/05/2025
Date Last Modified: 11/05/2025

Description:

Final Project

Source file for the SampleControlPanel class.
Implements all setup, event handling, and drawing for the
side panel used for loading and managing samples.
*/

#include "SampleControlPanel.h"

SampleControlPanel::SampleControlPanel(RenderWindow& window, const Font& font, const vector<SeqTrack*>& tracks, Vector2f panelSize, Vector2f panelPos)
    : window(window), font(font), tracks(tracks)
{
    // Panel Background
    panelBackground.setSize(panelSize);
    panelBackground.setFillColor(Color(40, 40, 40));
    panelBackground.setPosition(panelPos);

    this->numTracks = static_cast<int>(tracks.size());
    waveformDisplays.resize(numTracks);
    initShapes();
    loadSamples("samples");

    activeDropdown = -1; // -1 means no dropdown is active

    // Dropdown menu background (initially invisible)
    dropdownBackground.setFillColor(Color(20, 20, 20));
    dropdownBackground.setOutlineColor(Color::White);
    dropdownBackground.setOutlineThickness(1.0f);
}

void SampleControlPanel::handleMouse(Event event, float mousePosX, float mousePosY)
{
    if (activeDropdown != -1)
    {
        // If a dropdown is open, check for clicks on its items
        for (size_t i = 0; i < dropdownItems.size(); i++)
        {
            if (dropdownItems[i].getGlobalBounds().contains(mousePosX, mousePosY))
            {
                tracks[activeDropdown]->setSample(sampleBuffers[i]);
                sampleNameTexts[activeDropdown].setString(dropdownItems[i].getString());
                displayWaveform(activeDropdown, static_cast<int>(i));
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

void SampleControlPanel::update()
{
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

void SampleControlPanel::draw()
{
    window.draw(panelBackground);

    // Draw the background shading first
    for (const auto& bg : trackBackgrounds)
    {
        window.draw(bg);
    }

    for (int i = 0; i < numTracks; i++)
    {
        window.draw(trackLabels[i]);
        window.draw(dropdownButtons[i]);
        window.draw(sampleNameTexts[i]);
        window.draw(waveformDisplays[i]);
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

FloatRect SampleControlPanel::getGlobalBounds()
{
    return panelBackground.getGlobalBounds();
}

void SampleControlPanel::setupText(Text& text, const String& value, Vector2f position)
{
    text.setFont(font);
    text.setString(value);
    text.setCharacterSize(14);
    text.setFillColor(Color::White);
    text.setPosition(position);
}

void SampleControlPanel::initShapes()
{
    // Setup UI elements for each of the 8 tracks
    float trackHeight = panelBackground.getSize().y / static_cast<float>(numTracks);

    // Initialize all the GUI shapes
    trackBackgrounds.reserve(numTracks / 2);
    for (int i = 0; i < numTracks; i++)
    {
        RectangleShape bg;
        bg.setSize({panelBackground.getSize().x, trackHeight});
        bg.setPosition(panelBackground.getPosition().x, panelBackground.getPosition().y + i * trackHeight);
        bg.setOutlineColor(Color::Black);
        bg.setOutlineThickness(2.0f);
        if (i % 2 == 1) // Shade odd rows
        {
            bg.setFillColor(Color(50, 50, 50));
        }
        else
        {
            bg.setFillColor(Color(40, 40, 40));
        }
        trackBackgrounds.push_back(bg);
    }

    for (int i = 0; i < numTracks; i++)
    {
        float trackY = panelBackground.getPosition().y + i * trackHeight;

        // Track Label ("Track 1", "Track 2", etc.)
        Text label;
        setupText(label, "Track " + to_string(i + 1), {panelBackground.getPosition().x + 2, trackY + 5});
        trackLabels.push_back(label);

        // Dropdown Button
        RectangleShape button;
        button.setSize({panelBackground.getSize().x - 10, trackHeight / 5.0f});
        button.setPosition(panelBackground.getPosition().x + 2, trackY + 30);
        button.setFillColor(Color::Black);
        button.setOutlineColor(Color(80, 80, 80));
        button.setOutlineThickness(1.0f);
        dropdownButtons.push_back(button);

        // Sample Name Text (initially "No Sample")
        Text nameText;
        setupText(nameText, "No Sample", {button.getPosition().x + 5, button.getPosition().y + 2});
        sampleNameTexts.push_back(nameText);
    }
}

void SampleControlPanel::loadSamples(const string& directory)
{
    try {
        for (const auto& entry : fs::directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                string ext = entry.path().extension().string();
                if (ext == ".wav" || ext == ".flac" || ext == ".ogg")
                {
                    auto& path = entry.path().string();
                    string filename = fs::path(path).filename().string();

                    Text item;
                    setupText(item, filename, {0, 0});
                    dropdownItems.push_back(item);

                    SoundBuffer sampleBuffer;
                    if (sampleBuffer.loadFromFile(path))
                    {
                        sampleBuffers.push_back(sampleBuffer);
                        generateWaveform(sampleBuffer);
                    }
                    else
                    {
                        cerr << "Failed to load sample: " << path << endl;
                    }
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << endl;
    }
}

void SampleControlPanel::generateWaveform(SoundBuffer& buffer)
{
    VertexArray waveform(LineStrip);

    const Int16* samples = buffer.getSamples();
    Uint64 sampleCount = buffer.getSampleCount();

    float trackHeight = panelBackground.getSize().y / static_cast<float>(numTracks);
    float waveformWidth = panelBackground.getSize().x - 10;
    float waveformHeight = trackHeight * (3.0f / 5.0f);

    // Downsample to fit the display width
    Uint64 samplesPerPixel = sampleCount / static_cast<Uint64>(waveformWidth);
    if (samplesPerPixel == 0)
    {
        samplesPerPixel = 1;
    }

    for (unsigned int x = 0; x < static_cast<unsigned int>(waveformWidth); x++)
    {
        Uint64 sampleIndex = x * samplesPerPixel;
        if (sampleIndex >= sampleCount)
        {
            break;
        }
        float sampleValue = static_cast<float>(samples[sampleIndex]) / 32768.0f; // Normalize to [-1, 1]
        float y = (waveformHeight / 2.0f) * (1 - sampleValue);
        waveform.append(Vertex({panelBackground.getPosition().x + 10 + x, y}, Color::Cyan));
    }
    sampleWaveforms.push_back(waveform);
}

void SampleControlPanel::displayWaveform(int trackIndex, int bufferIndex)
{
    VertexArray& sourceWaveform = sampleWaveforms[bufferIndex];
    VertexArray& targetDisplay = waveformDisplays[trackIndex];
    targetDisplay.clear();
    targetDisplay.setPrimitiveType(LineStrip);

    float waveformYOffset = dropdownButtons[trackIndex].getPosition().y + 15; // Position below other controls
    for (size_t i = 0; i < sourceWaveform.getVertexCount(); i++) {
        Vertex v = sourceWaveform[i];
        v.position.y += waveformYOffset;
        targetDisplay.append(v);
    }
}

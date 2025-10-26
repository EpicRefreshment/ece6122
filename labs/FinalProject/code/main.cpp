#include <SFML/Graphics.hpp>
#include "SequencerEngine.h"

int main() 
{
    // --- Window and GUI Setup ---
    const int windowWidth = 800;
    const int windowHeight = 600;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Multimode Sequencer");
    window.setFramerateLimit(60);

    const int numTracks = 8;
    const int numSteps = 16;
    const float padding = 10.f;
    const float stepWidth = (windowWidth - (padding * (numSteps + 1))) / numSteps;
    const float stepHeight = (windowHeight / numTracks) - (padding * 2);

    std::vector<sf::RectangleShape> stepShapes(numSteps * numTracks);
    for (int i = 0; i < (numSteps * numTracks); i++) {
        stepShapes[i].setSize(sf::Vector2f(stepWidth, stepHeight));
        stepShapes[i].setPosition(padding + i * (stepWidth + padding), padding + (stepHeight * (i / numSteps)));
        stepShapes[i].setOutlineThickness(2.f);
        stepShapes[i].setOutlineColor(sf::Color(80, 80, 80));
    }

    // --- Sequencer Initialization ---
    SequencerEngine engine;

    // --- Main Application Loop ---
    while (window.isOpen()) {
        // --- Event Handling ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Start/Stop with the space bar
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                if (engine.isPlaying()) {
                    engine.stop();
                } else {
                    engine.play();
                }
            }
            
            // Toggle steps with mouse click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < numSteps; ++i) {
                    if (stepShapes[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        engine.toggleStep(i);
                    }
                }
            }
        }

        // --- Logic Update ---
        engine.update();

        // --- Drawing ---
        window.clear(sf::Color(30, 30, 30)); // Dark grey background

        const std::vector<bool>& gridState = engine.getGridState();
        int currentStep = engine.getCurrentStep();

        for (int i = 0; i < (numSteps * numTracks); i++) {
            // Set color based on state
            if (i == currentStep) {
                stepShapes[i].setFillColor(sf::Color::Yellow);
            } else if (gridState[i]) {
                stepShapes[i].setFillColor(sf::Color::Cyan);
            } else {
                stepShapes[i].setFillColor(sf::Color::Black);
            }
            window.draw(stepShapes[i]);
        }
        
        window.display();
    }

    return 0;
}
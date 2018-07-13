#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include "Camera.hpp"

int main(int, char const**)
{
    Camera camera;
    
    sf::Clock clock;
    int frameCounter = 0;
    
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "bncuword.ttf")) {
        return EXIT_FAILURE;
    }
    
    sf::Text text("", font, 14);
    text.setFillColor(sf::Color::Black);
    text.setPosition(0, 0);
    
    const int CANVAS_WIDTH = 400;
    const int CANVAS_HEIGHT = 250;
    
    sf::RenderWindow window(sf::VideoMode(CANVAS_WIDTH, CANVAS_HEIGHT), "SFML window");
    window.setVerticalSyncEnabled(true);

    sf::Texture screen;
    screen.create(CANVAS_WIDTH, CANVAS_HEIGHT);
    sf::Sprite sprite(screen);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        camera.update();
        camera.clearView();
        camera.viewScene();
        
        window.clear();
        screen.update(camera.getPixels());
        window.draw(sprite);
        window.draw(text);
        window.display();
        
        if (clock.getElapsedTime().asSeconds() >= 1.0) {
            clock.restart();
            text.setString(std::to_string(frameCounter));
            frameCounter = 0;
        }
        frameCounter++;
    }

    return EXIT_SUCCESS;
}

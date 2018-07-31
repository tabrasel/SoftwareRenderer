#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

class Vertex
{
    private:
        sf::Vector3f objectPosition;
        sf::Vector3f worldPosition;
        sf::Vector3f cameraPosition;
        sf::Vector2f screenPosition;
    
    public:
        Vertex();
        Vertex(sf::Vector3f& worldPosition);
    
        // Setters
        void setCameraPosition(sf::Vector3f& cameraPosition);
        void setScreenPosition(sf::Vector2f& screenPosition);
    
        // Getters
        sf::Vector3f& getWorldPosition();
        sf::Vector3f& getCameraPosition();
        sf::Vector2f& getScreenPosition();
};

#endif

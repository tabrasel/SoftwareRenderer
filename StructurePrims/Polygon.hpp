#ifndef Polygon_hpp
#define Polygon_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "Vertex.hpp"
#include <array>

class Polygon
{
    private:
        std::array<Vertex*, 3> vertices;
        std::array<sf::Vector2f*, 3> textureCoords;
    
    public:
        Polygon();
        Polygon(Vertex* v1, Vertex* v2, Vertex* v3);
        Polygon(std::array<Vertex*, 3>&, std::array<sf::Vector2f*, 3>&);
        std::array<Vertex*, 3>& getVertices();
        std::array<sf::Vector2f*, 3>& getTextureCoords();
};

#endif

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include "Vertex.hpp"
#include "Polygon.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class Mesh
{
    private:
        std::vector<Polygon*> polygons;
        std::vector<Vertex*> vertices;
        std::vector<sf::Vector2f*> textureCoords;
        sf::Image texture;
    
    public:
        Mesh();
        std::vector<Polygon*>& getPolygons();
        std::vector<Vertex*>& getVertices();
        sf::Image& getTexture();
};

#endif

#include "Polygon.hpp"
#include <iostream>

Polygon::Polygon(Vertex* v1, Vertex* v2, Vertex* v3)
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
}

Polygon::Polygon(std::array<Vertex*, 3>& vertices, std::array<sf::Vector2f*, 3>& textureCoords)
{
    this->vertices = vertices;
    this->textureCoords = textureCoords;
}

std::array<Vertex*, 3>& Polygon::getVertices()
{
    return vertices;
}

std::array<sf::Vector2f*, 3>& Polygon::getTextureCoords()
{
    return textureCoords;
}

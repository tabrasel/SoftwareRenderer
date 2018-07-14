#include "Polygon.hpp"

Polygon::Polygon()
{
    sf::Vector3f point1(0, 8, 6);
    sf::Vector3f point2(-2, 3, 3);
    sf::Vector3f point3(2, 1, -1);
    
    // Reverse order just for fun
    vertices[0] = Vertex(point3);
    vertices[1] = Vertex(point2);
    vertices[2] = Vertex(point1);
}

std::array<Vertex, 3>& Polygon::getVertices()
{
    return vertices;
}

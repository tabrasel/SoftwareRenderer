#include "Polygon.hpp"

Polygon::Polygon()
{
    sf::Vector3f point1(-1, 2, 2);
    sf::Vector3f point2(1, 1, 2);
    sf::Vector3f point3(-1, 1, 2);
    
    vertices[0] = Vertex(point1);
    vertices[1] = Vertex(point2);
    vertices[2] = Vertex(point3);
}

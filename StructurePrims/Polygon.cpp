#include "Polygon.hpp"
#include <iostream>

Polygon::Polygon(Vertex* v1, Vertex* v2, Vertex* v3)
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
}

std::array<Vertex*, 3>& Polygon::getVertices()
{
    return vertices;
}

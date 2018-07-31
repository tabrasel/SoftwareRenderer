#ifndef Polygon_hpp
#define Polygon_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "Vertex.hpp"
#include <array>

class Polygon
{
    private:
        std::array<Vertex, 3> vertices;
    
    public:
        Polygon();
        Polygon(Vertex& v1, Vertex& v2, Vertex& v3);
        std::array<Vertex, 3>& getVertices();
};

#endif

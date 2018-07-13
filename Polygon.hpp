#ifndef Polygon_hpp
#define Polygon_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "Vertex.hpp"

class Polygon
{
    private:
    Vertex vertices[3];
    
    public:
        Polygon();
        Vertex* getVertices;
};

#endif

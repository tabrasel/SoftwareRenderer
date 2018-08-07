#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include "Vertex.hpp"
#include "Polygon.hpp"
#include <vector>

// At some point include edges?

class Mesh
{
    private:
        std::vector<Polygon*> polygons;
        std::vector<Vertex*> vertices;
    
    public:
        Mesh();
        std::vector<Polygon*>& getPolygons();
        std::vector<Vertex*>& getVertices();
};

#endif

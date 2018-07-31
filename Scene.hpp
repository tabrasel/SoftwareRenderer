#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "Polygon.hpp"
#include <vector>
#include "Mesh.hpp"

class Scene
{
    private:
        Polygon polygon;
        Mesh mesh;
        std::vector<Polygon> polygons;
    
    public:
        Scene();
        Polygon& getPolygon();
        Mesh& getMesh();
};

#endif

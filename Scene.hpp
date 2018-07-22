#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "Polygon.hpp"
#include <vector>

class Scene
{
    private:
        Polygon polygon;
        std::vector<Polygon> polygons;
    
    public:
        Scene();
        Polygon& getPolygon();
        std::vector<Polygon>* getPolygons;
};

#endif

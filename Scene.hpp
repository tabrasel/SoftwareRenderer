#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "Polygon.hpp"

class Scene
{
    private:
        Polygon polygon;
    
    public:
        Scene();
        Polygon& getPolygon();
};

#endif

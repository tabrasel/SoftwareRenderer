#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "Polygon.hpp"
#include <vector>
#include "Mesh.hpp"

class Scene
{
    private:
        Mesh mesh;
    
    public:
        Scene();
        Mesh& getMesh();
};

#endif

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>
#include "Vertex.hpp"
#include <SFML/Graphics.hpp>

class Edge
{
    private:
        Vertex start, end;
        double startY, endY;
        double x, xStep;
        double z, zStep;
    double u, uStep;
    double v, vStep;
    
    public:
        Edge(Vertex& start, Vertex& end);
        void step();
        double getStartY();
        double getEndY();
        double getX();
        double getZ();
    double getU();
    double getV();
};

#endif

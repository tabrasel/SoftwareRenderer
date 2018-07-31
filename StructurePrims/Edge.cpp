#include "Edge.hpp"
#include <cmath>
#include <iostream>

Edge::Edge(Vertex& start, Vertex& end)
{
    this->start = start;
    this->end = end;
    
    double yDelta = end.getScreenPosition().y - start.getScreenPosition().y;
    
    startY = std::ceil(start.getScreenPosition().y - 0.5) + 0.5;
    endY = std::ceil(end.getScreenPosition().y - 0.5) + 0.5;
    
    xStep = (end.getScreenPosition().x - start.getScreenPosition().x) / yDelta;
    x = start.getScreenPosition().x + xStep * (startY - start.getScreenPosition().y);
    
    zStep = ((1.0 / end.getCameraPosition().z) - (1.0 / start.getCameraPosition().z)) / yDelta;
    z = (1.0 / start.getCameraPosition().z) + zStep * (startY - start.getScreenPosition().y);
}

void Edge::step()
{
    x += xStep;
    z += zStep;
}

double Edge::getStartY()
{
    return startY;
}

double Edge::getEndY()
{
    return endY;
}

double Edge::getX()
{
    return x;
}

double Edge::getZ()
{
    return z;
}

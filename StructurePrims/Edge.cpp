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
    double offset = (startY - start.getScreenPosition().y);
    
    xStep = (end.getScreenPosition().x - start.getScreenPosition().x) / yDelta;
    x = start.getScreenPosition().x + xStep * offset;
    
    zStep = ((1.0 / end.getCameraPosition().z) - (1.0 / start.getCameraPosition().z)) / yDelta;
    z = (1.0 / start.getCameraPosition().z) + zStep * offset;
    
    uStep = ((end.getTextureCoords().x / end.getCameraPosition().z) - (start.getTextureCoords().x / start.getCameraPosition().z)) / yDelta;
    u = (start.getTextureCoords().x / start.getCameraPosition().z) + uStep * offset;
    
    vStep = ((end.getTextureCoords().y / end.getCameraPosition().z) - (start.getTextureCoords().y / start.getCameraPosition().z)) / yDelta;
    v = (start.getTextureCoords().y / start.getCameraPosition().z) + vStep * offset;
    
    
}

void Edge::step()
{
    x += xStep;
    z += zStep;
    u += uStep;
    v += vStep;
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

double Edge::getU()
{
    return u;
}

double Edge::getV()
{
    return v;
}

#include "Scene.hpp"

Scene::Scene()
{
    polygon = Polygon();
}

Polygon& Scene::getPolygon()
{
    return polygon;
}

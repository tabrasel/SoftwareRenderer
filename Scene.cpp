#include "Scene.hpp"

Scene::Scene()
{
    polygon = Polygon();
    mesh = Mesh();
}

Mesh& Scene::getMesh()
{
    return mesh;
}

Polygon& Scene::getPolygon()
{
    return polygon;
}

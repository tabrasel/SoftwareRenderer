#include "Vertex.hpp"

Vertex::Vertex()
{
    objectPosition  = sf::Vector3f(0.0, 0.0, 0.0);
    worldPosition   = sf::Vector3f(0.0, 0.0, 0.0);
    cameraPosition  = sf::Vector3f(0.0, 0.0, 0.0);
    screenPosition  = sf::Vector2f(0.0, 0.0);
}

Vertex::Vertex(sf::Vector3f& worldPosition)
{
    this->worldPosition = worldPosition;
}

void Vertex::setCameraPosition(sf::Vector3f& cameraPosition)
{
    this->cameraPosition = cameraPosition;
}

void Vertex::setScreenPosition(sf::Vector2f& screenPosition)
{
    this->screenPosition = screenPosition;
}

sf::Vector3f& Vertex::getWorldPosition()
{
    return worldPosition;
}

sf::Vector3f& Vertex::getCameraPosition()
{
    return cameraPosition;
}

sf::Vector2f& Vertex::getScreenPosition()
{
    return screenPosition;
}

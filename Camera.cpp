#include "Camera.hpp"
#include "Edge.hpp"
#include "Scene.hpp"
#include "Quaternion.hpp"

#include <iostream>
#include <cmath>

Camera::Camera()
{
    position = sf::Vector3f(0.0, 1.0, -4.0);
    angle = sf::Vector3f(0.0, 0.0, 0.0);
    viewSize = sf::Vector2f(400, 250);
    pixels = new sf::Uint8[int(viewSize.x * viewSize.y * 4)];
    zBuffer = new double[int(viewSize.x * viewSize.y)];
    nearPlane = 0.1;
    sf::Mouse::setPosition(sf::Vector2i(1280, 800));
    lastMousePos = sf::Vector2i(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
}

Camera::~Camera()
{
    delete pixels;
    delete zBuffer;
}

void Camera::update()
{
    // Position
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        position += sf::Vector3f(0.0, 0.0, 0.05);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        position += sf::Vector3f(0.0, 0.0, -0.05);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        position += sf::Vector3f(-0.05, 0.0, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        position += sf::Vector3f(0.05, 0.0, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        position += sf::Vector3f(0.0, 0.05, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        position += sf::Vector3f(0.0, -0.05, 0.0);
    }
    
    // Angle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        angle += sf::Vector3f(0.0, 0.02, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        angle += sf::Vector3f(0.0, -0.02, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        angle += sf::Vector3f(0.02, 0.0, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        angle += sf::Vector3f(-0.02, 0.0, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        angle += sf::Vector3f(0.0, 0.0, -0.05);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        angle += sf::Vector3f(0.0, 0.0, 0.05);
    }
    
    sf::Vector2i mouseDelta(sf::Mouse::getPosition().x - 1280, sf::Mouse::getPosition().y - 1600 - 800);
    lastMousePos.x = sf::Mouse::getPosition().x;
    lastMousePos.y = sf::Mouse::getPosition().y;
    angle += sf::Vector3f(-mouseDelta.y * 0.002, -mouseDelta.x * 0.002, 0.0);
    sf::Mouse::setPosition(sf::Vector2i(1280, 800));
}

void Camera::clearView()
{
    for (int y = 0; y < viewSize.y; y++)
    {
        for (int x = 0; x < viewSize.x; x++)
        {
            sf::Color color(255, 255, 255);
            putPixel(x, y, color);
        }
    }
}

void Camera::viewScene(Scene& scene)
{
    std::array<Vertex, 3> vertices = scene.getPolygon().getVertices();
    
    sf::Vector3f yawAxis = sf::Vector3f(0.0, 1.0, 0.0);
    sf::Vector3f pitchAxis = sf::Vector3f(1.0, 0.0, 0.0);
    sf::Vector3f rollAxis = sf::Vector3f(0.0, 0.0, 1.0);
    Quaternion yawRot = Quaternion(yawAxis, angle.y);
    Quaternion pitchRot = Quaternion(pitchAxis, angle.x);
    Quaternion rollRot = Quaternion(rollAxis, angle.z);
    
    // Draw grid
    for (double z = -10; z <= 10; z += 1)
    {
        for (double x = -10; x <= 10; x += 1)
        {
            sf::Vector3f relPos = sf::Vector3f(x - position.x, 0 - position.y, z - position.z);
            sf::Vector3f* camPos = yawRot.rotateVector(relPos);
            sf::Vector3f cameraPosition = *camPos;
            camPos = pitchRot.rotateVector(cameraPosition);
            cameraPosition = *camPos;
            camPos = rollRot.rotateVector(cameraPosition);
            cameraPosition = *camPos;
            
            sf::Vector2f screenPosition = sf::Vector2f(viewSize.x / 2 + (cameraPosition.x / cameraPosition.z) * viewSize.x / 2,
                                                       viewSize.y / 2 - (cameraPosition.y / cameraPosition.z) * viewSize.y / 2);
            if (cameraPosition.z > 0) {
                sf::Color color(0, 0, 0);
                putPixel((int)screenPosition.x, (int)screenPosition.y, color);
            }
            delete camPos;
        }
    }
    
    // Transform vertices to camera and screen coordinates
    for (int i = 0; i < vertices.size(); i++)
    {
        sf::Vector3f relPos = vertices[i].getWorldPosition() - position;
        sf::Vector3f* camPos = yawRot.rotateVector(relPos);
        sf::Vector3f cameraPosition = *camPos;
        camPos = pitchRot.rotateVector(cameraPosition);
        cameraPosition = *camPos;
        camPos = rollRot.rotateVector(cameraPosition);
        cameraPosition = *camPos;
        
        vertices[i].setCameraPosition(cameraPosition);
        sf::Vector2f screenPosition = sf::Vector2f(viewSize.x / 2 + (cameraPosition.x / cameraPosition.z) * viewSize.x / 2,
                                                   viewSize.y / 2 - (cameraPosition.y / cameraPosition.z) * viewSize.y / 2);
        vertices[i].setScreenPosition(screenPosition);
        delete camPos;
    }
    
    if (vertices[0].getCameraPosition().z <= nearPlane || vertices[1].getCameraPosition().z <= nearPlane || vertices[2].getCameraPosition().z <= nearPlane)
    {
        return;
    }
    
    Vertex* top = &vertices[0];
    Vertex* mid = &vertices[1];
    Vertex* bot = &vertices[2];
    
    if (bot->getScreenPosition().y < mid->getScreenPosition().y) {
        Vertex* temp = mid;
        mid = bot;
        bot = temp;
    }
    if (mid->getScreenPosition().y < top->getScreenPosition().y) {
        Vertex* temp = top;
        top = mid;
        mid = temp;
    }
    if (bot->getScreenPosition().y < mid->getScreenPosition().y) {
        Vertex* temp = mid;
        mid = bot;
        bot = temp;
    }
    
    // Create edges
    Edge topToMid = Edge(*top, *mid);
    Edge midToBot = Edge(*mid, *bot);
    Edge topToBot = Edge(*top, *bot);
    
    // Is the long side on the left?
    bool isLeftHanded = polygonLeftHanded(*top, *mid, *bot);
    
    Edge* leftEdge = &topToMid;
    Edge* rightEdge = &topToBot;
    if (!isLeftHanded)
    {
        leftEdge = &topToBot;
        rightEdge = &topToMid;
    }
    
    sf::Color fillColor(0, 0, 255);
    
    // Draw top triangle
    for (int y = (int)topToMid.getStartY(); y < (int)topToMid.getEndY(); y++)
    {
        int startX = std::ceil(leftEdge->getX() - 0.5);
        int endX = std::ceil(rightEdge->getX() - 1.5);
        
        double sliceZStep = (rightEdge->getZ() - leftEdge->getZ()) / (rightEdge->getX() - leftEdge->getX());
        double sliceZ = leftEdge->getZ() + ((std::ceil(leftEdge->getX() - 0.5) + 0.5) - leftEdge->getX()) * sliceZStep;
        
        for (int x = startX; x < endX; x++)
        {
            double z = 1.0 / sliceZ;
            double u = z * 20;
            if (u < 0) {
                u = 0;
            }
            if (u > 255) {
                u = 255;
            }
            sf::Color fillColor(u, 0, 255);
            putPixel(x, y, fillColor);
            sliceZ += sliceZStep;
        }
        
        leftEdge->step();
        rightEdge->step();
    }
    
    leftEdge = &midToBot;
    rightEdge = &topToBot;
    if (!isLeftHanded)
    {
        leftEdge = &topToBot;
        rightEdge = &midToBot;
    }
    
    // Draw bottom triangle
    for (int y = (int)midToBot.getStartY(); y < (int)midToBot.getEndY(); y++)
    {
        int startX = std::ceil(leftEdge->getX() - 0.5);
        int endX = std::ceil(rightEdge->getX() - 1.5);
        
        double sliceZStep = (rightEdge->getZ() - leftEdge->getZ()) / (rightEdge->getX() - leftEdge->getX());
        double sliceZ = leftEdge->getZ() + ((std::ceil(leftEdge->getX() - 0.5) + 0.5) - leftEdge->getX()) * sliceZStep;
        
        for (int x = startX; x < endX; x++)
        {
            double z = 1.0 / sliceZ;
            double u = z * 20;
            if (u < 0) {
                u = 0;
            }
            if (u > 255) {
                u = 255;
            }
            sf::Color fillColor(u, 0, 255);
            putPixel(x, y, fillColor);
            sliceZ += sliceZStep;
        }
        
        leftEdge->step();
        rightEdge->step();
    }
    
    // Draw crosshair
    sf::Color color(255, 0, 0);
    putPixel(199, 125, color);
    putPixel(201, 125, color);
    putPixel(200, 124, color);
    putPixel(200, 126, color);
}

void Camera::putPixel(int x, int y, sf::Color& color)
{
    if (x >= 0 && x < viewSize.x && y >= 0 && y < viewSize.y) {
        int index = (x + y * viewSize.x) * 4;
        pixels[index]     = color.r;    // Red
        pixels[index + 1] = color.g;    // Green
        pixels[index + 2] = color.b;    // Blue
        pixels[index + 3] = color.a;    // Alpha
    }
}

bool Camera::polygonLeftHanded(Vertex& top, Vertex& middle, Vertex& bottom)
{
    double x1 = middle.getScreenPosition().x - top.getScreenPosition().x;
    double y1 = middle.getScreenPosition().y - top.getScreenPosition().y;
    
    sf::Vector2f topToMiddle = middle.getScreenPosition() - top.getScreenPosition();
    sf::Vector2f topToBottom = bottom.getScreenPosition() - top.getScreenPosition();
    
    return (topToMiddle.x * topToBottom.y - topToBottom.x * topToMiddle.y) < 0;
}

sf::Vector3f& Camera::getPosition()
{
    return position;
}

sf::Uint8* Camera::getPixels()
{
    return pixels;
}

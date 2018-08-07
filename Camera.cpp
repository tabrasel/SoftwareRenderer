#include "Camera.hpp"
#include "Quaternion.hpp"
#include "Mesh.hpp"

#include <iostream>
#include <cmath>

Camera::Camera()
{
    position = sf::Vector3f(0.0, 1.0, -4.0);
    angle = sf::Vector3f(0.0, 0.0, 0.0);
    
    forward = sf::Vector3f(0.0, 0.0, 1.0);
    sideways = sf::Vector3f(1.0, 0.0, 0.0);
    upward = sf::Vector3f(0.0, 1.0, 0.0);
    
    viewSize = sf::Vector2f(400, 255);
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
    sf::Vector3f yawAxis = sf::Vector3f(0.0, 1.0, 0.0);
    sf::Vector3f pitchAxis = sf::Vector3f(1.0, 0.0, 0.0);
    sf::Vector3f rollAxis = sf::Vector3f(0.0, 0.0, 1.0);
    
    Quaternion yawRot = Quaternion(yawAxis, angle.y);
    Quaternion pitchRot = Quaternion(pitchAxis, angle.x);
    Quaternion rollRot = Quaternion(rollAxis, angle.z);
    
    rollRot.rotateVector(forward);
    pitchRot.rotateVector(forward);
    yawRot.rotateVector(forward);
    
    rollRot.rotateVector(sideways);
    pitchRot.rotateVector(sideways);
    yawRot.rotateVector(sideways);
    
    rollRot.rotateVector(upward);
    pitchRot.rotateVector(upward);
    yawRot.rotateVector(upward);
    
    // Position
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        position.x += forward.x * 0.05;
        position.y += forward.y * 0.05;
        position.z += forward.z * 0.05;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        position.x += forward.x * -0.05;
        position.y += forward.y * -0.05;
        position.z += forward.z * -0.05;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        //position += sf::Vector3f(-0.05, 0.0, 0.0);
        position.x += sideways.x * -0.05;
        position.y += sideways.y * -0.05;
        position.z += sideways.z * -0.05;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        //position += sf::Vector3f(0.05, 0.0, 0.0);
        position.x += sideways.x * 0.05;
        position.y += sideways.y * 0.05;
        position.z += sideways.z * 0.05;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        //position += sf::Vector3f(0.0, 0.05, 0.0);
        position.x += upward.x * 0.05;
        position.y += upward.y * 0.05;
        position.z += upward.z * 0.05;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        position += sf::Vector3f(0.0, -0.05, 0.0);
    }
    
    // Angle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        angle += sf::Vector3f(0.0, -0.05, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        angle += sf::Vector3f(0.0, 0.05, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        angle += sf::Vector3f(-0.05, 0.0, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        angle += sf::Vector3f(0.05, 0.0, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        angle += sf::Vector3f(0.0, 0.0, 0.05);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        angle += sf::Vector3f(0.0, 0.0, -0.05);
    }
    
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::T))
    {
        sf::Vector2i mouseDelta(sf::Mouse::getPosition().x - 1280, sf::Mouse::getPosition().y - 1600 - 800);
        lastMousePos.x = sf::Mouse::getPosition().x;
        lastMousePos.y = sf::Mouse::getPosition().y;
        angle += sf::Vector3f(mouseDelta.y * 0.002, mouseDelta.x * 0.002, 0.0);
        sf::Mouse::setPosition(sf::Vector2i(1280, 800));
    }
    
    forward = sf::Vector3f(0.0, 0.0, 1.0);
    sideways = sf::Vector3f(1.0, 0.0, 0.0);
    upward = sf::Vector3f(0.0, 1.0, 0.0);
}

void Camera::clearView()
{
    sf::Color color(255, 255, 255);
    int index = 0;
    for (int y = 0; y < viewSize.y; y++)
    {
        for (int x = 0; x < viewSize.x; x++)
        {
            putPixel(x, y, color);
            zBuffer[index] = 99999;
            index++;
        }
    }
}

void Camera::viewScene(Scene& scene)
{
    sf::Vector3f yawAxis = sf::Vector3f(0.0, 1.0, 0.0);
    sf::Vector3f pitchAxis = sf::Vector3f(1.0, 0.0, 0.0);
    sf::Vector3f rollAxis = sf::Vector3f(0.0, 0.0, 1.0);
    
    Quaternion yawRot = Quaternion(yawAxis, -angle.y);
    Quaternion pitchRot = Quaternion(pitchAxis, -angle.x);
    Quaternion rollRot = Quaternion(rollAxis, -angle.z);
    
    // Draw grid
    for (double z = -10; z <= 10; z += 1)
    {
        for (double x = -10; x <= 10; x += 1)
        {
            sf::Vector3f camPos = sf::Vector3f(x - position.x, 0 - position.y, z - position.z);
            yawRot.rotateVector(camPos);
            pitchRot.rotateVector(camPos);
            rollRot.rotateVector(camPos);
            sf::Vector2f screenPosition = sf::Vector2f(viewSize.x / 2 + (camPos.x / camPos.z) * viewSize.x / 2,
                                                       viewSize.y / 2 - (camPos.y / camPos.z) * viewSize.y / 2);
            if (camPos.z > 0) {
                sf::Color color(0, 0, 0);
                putPixel((int)screenPosition.x, (int)screenPosition.y, color);
            }
        }
    }
    
    
    // Draw meshes
    Mesh mesh = scene.getMesh();
    std::vector<Vertex*> vertices = mesh.getVertices();
    std::vector<Polygon*> polygons = mesh.getPolygons();
    
    // Transform vertices to camera and screen coordinates
    for (int i = 0; i < vertices.size(); i++)
    {
        Vertex* vertex = vertices[i];
        
        sf::Vector3f camPos = vertex->getWorldPosition() - position;
        
        yawRot.rotateVector(camPos);
        pitchRot.rotateVector(camPos);
        rollRot.rotateVector(camPos);
        
        vertex->setCameraPosition(camPos);
        sf::Vector2f screenPosition = sf::Vector2f(viewSize.x / 2 + (camPos.x / camPos.z) * viewSize.x / 2,
                                                   viewSize.y / 2 - (camPos.y / camPos.z) * viewSize.y / 2);
        vertex->setScreenPosition(screenPosition);
    }
    
    /*
    if (vertices[0].getCameraPosition().z <= nearPlane || vertices[1].getCameraPosition().z <= nearPlane || vertices[2].getCameraPosition().z <= nearPlane)
    {
        return;
    }
     */
    
    
    for (int i = 0; i < polygons.size(); i++)
    {
        Polygon* polygon = polygons[i];
        
        Vertex* top  = polygon->getVertices()[0];
        Vertex* mid  = polygon->getVertices()[1];
        Vertex* bot  = polygon->getVertices()[2];

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
        
        drawTriangleHalf((int)topToMid.getStartY(), (int)topToMid.getEndY(), leftEdge, rightEdge);
        
        leftEdge = &midToBot;
        rightEdge = &topToBot;
        if (!isLeftHanded)
        {
            leftEdge = &topToBot;
            rightEdge = &midToBot;
        }
        
        drawTriangleHalf((int)midToBot.getStartY(), (int)midToBot.getEndY(), leftEdge, rightEdge);
    }
}

void Camera::drawTriangleHalf(int topY, int bottomY, Edge* leftEdge, Edge* rightEdge)
{
    for (int y = topY; y < bottomY; y++)
    {
        // Scanline left and right boundries
        int startX = std::ceil(leftEdge->getX() - 0.5);
        int endX = std::ceil(rightEdge->getX() - 1.5);
        
        // Scanline Z interpolants
        double sliceZStep = (rightEdge->getZ() - leftEdge->getZ()) / (rightEdge->getX() - leftEdge->getX());
        double sliceZ = leftEdge->getZ() + ((std::ceil(leftEdge->getX() - 0.5) + 0.5) - leftEdge->getX()) * sliceZStep;
        
        for (int x = startX; x < endX; x++)
        {
            double z = 1.0 / sliceZ;
            int index = y * viewSize.x + x;
            
            if (x >= 0 && x < viewSize.x && y >= 0 && y < viewSize.y)
            {
                if (z < zBuffer[index]) {
                    double u = z * 20;
                    if (u < 0) {
                        u = 0;
                    }
                    if (u > 255) {
                        u = 255;
                    }
                    
                    sf::Color fillColor(u, 0, 255);
                    putPixel(x, y, fillColor);
                    zBuffer[index] = z;
                }
            }
            
            if (index >= 0 && index < viewSize.x * viewSize.y)
            {
                
            }
            
            sliceZ += sliceZStep;
        }
        
        leftEdge->step();
        rightEdge->step();
    }
}

void Camera::putPixel(int x, int y, sf::Color& color)
{
    if (x >= 0 && x < viewSize.x && y >= 0 && y < viewSize.y)
    {
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

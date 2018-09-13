#include "Camera.hpp"
#include "Quaternion.hpp"
#include "Mesh.hpp"
#include "ResourcePath.hpp"

#include <iostream>
#include <cmath>

Camera::Camera()
{
    position = sf::Vector3f(0.0, 1.0, -4.0);
    angle = sf::Vector3f(0.0, 0.0, 0.0);
    
    forward = sf::Vector3f(0.0, 0.0, 1.0);
    sideways = sf::Vector3f(1.0, 0.0, 0.0);
    upward = sf::Vector3f(0.0, 1.0, 0.0);
    
    fov = sf::Vector2f(1.570796, 1.2);
    viewSize = sf::Vector2f(400, 255);
    
    frustumStep = sf::Vector2f(tan(fov.x / 2), tan(fov.y / 2));
    focalLength = sf::Vector2f((viewSize.x / 2) / frustumStep.x, (viewSize.y / 2) / frustumStep.y);
    
    pixels = new sf::Uint8[int(viewSize.x * viewSize.y * 4)];
    zBuffer = new double[int(viewSize.x * viewSize.y)];
    nearPlane = 0.2;
    
    sf::Mouse::setPosition(sf::Vector2i(1280, 800));
    lastMousePos = sf::Vector2i(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
    
    texture.loadFromFile(resourcePath() + "pizzatex.jpg");
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
        //position += sf::Vector3f(0.0, -0.05, 0.0);
        position.x += upward.x * -0.05;
        position.y += upward.y * -0.05;
        position.z += upward.z * -0.05;
    }
    
    // Angle
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
        angle += sf::Vector3f(mouseDelta.y * 0.001, mouseDelta.x * 0.001, 0.0);
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
    for (double z = -5; z <= 5; z += 1)
    {
        for (double x = -5; x <= 5; x += 1)
        {
            sf::Vector3f camPos = sf::Vector3f(x - position.x, 0 - position.y, z - position.z);
            yawRot.rotateVector(camPos);
            pitchRot.rotateVector(camPos);
            rollRot.rotateVector(camPos);
            
            sf::Vector2f screenPosition = sf::Vector2f(viewSize.x / 2 + ((camPos.x * focalLength.x) / camPos.z),
                                                       viewSize.y / 2 - ((camPos.y * focalLength.y) / camPos.z));
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

        sf::Vector2f screenPosition = sf::Vector2f(viewSize.x / 2 + ((camPos.x * focalLength.x) / camPos.z),
                                                   viewSize.y / 2 - ((camPos.y * focalLength.y) / camPos.z));

        vertex->setScreenPosition(screenPosition);
    }
    
    int drawnPolys = 0;
    
    for (int i = 0; i < polygons.size(); i++)
    {
        
        Polygon* polygon = polygons[i];
        
        Vertex* top = polygon->getVertices()[0];
        Vertex* mid = polygon->getVertices()[1];
        Vertex* bot = polygon->getVertices()[2];
        
        // Set triangle texture coordinates to corresponsiding vertices
        top->setTextureCoords(*(polygon->getTextureCoords()[0]));
        mid->setTextureCoords(*(polygon->getTextureCoords()[1]));
        bot->setTextureCoords(*(polygon->getTextureCoords()[2]));
        
        if (triangleInFrustum(top, mid, bot))
        {
            // Determine if triangle is front or back facing
            sf::Vector3f edge1 = sf::Vector3f(mid->getWorldPosition().x - top->getWorldPosition().x,
                                              mid->getWorldPosition().y - top->getWorldPosition().y,
                                              mid->getWorldPosition().z - top->getWorldPosition().z);
            sf::Vector3f edge2 = sf::Vector3f(bot->getWorldPosition().x - top->getWorldPosition().x,
                                              bot->getWorldPosition().y - top->getWorldPosition().y,
                                              bot->getWorldPosition().z - top->getWorldPosition().z);
            float normalX = edge1.y * edge2.z - edge1.z * edge2.y;
            float normalY = edge1.z * edge2.x - edge1.x * edge2.z;
            float normalZ = edge1.x * edge2.y - edge1.y * edge2.x;
            sf::Vector3f normal = sf::Vector3f(normalX, normalY, normalZ);
            sf::Vector3f cameraToPolygon = sf::Vector3f(top->getWorldPosition() - position);
            float angleDiff = normal.x * cameraToPolygon.x + normal.y * cameraToPolygon.y + normal.z * cameraToPolygon.z;
            
            // Only draw front-facing triangles
            if (angleDiff < 0)
            {
                drawnPolys++;
                
                // Order vertices from top to bottom
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
                
                // Determine left and right edges of top triangle
                Edge* leftEdge = &topToMid;
                Edge* rightEdge = &topToBot;
                if (!isLeftHanded)
                {
                    leftEdge = &topToBot;
                    rightEdge = &topToMid;
                }
                
                // Draw top triangle
                drawTriangleHalf((int)topToMid.getStartY(), (int)topToMid.getEndY(), leftEdge, rightEdge);
                
                // Determine left and right edges of bottom triangle
                leftEdge = &midToBot;
                rightEdge = &topToBot;
                if (!isLeftHanded)
                {
                    leftEdge = &topToBot;
                    rightEdge = &midToBot;
                }
                
                // Draw bottom triangle
                drawTriangleHalf((int)midToBot.getStartY(), (int)midToBot.getEndY(), leftEdge, rightEdge);
            }
        }
        
    }
}

void Camera::drawTriangleHalf(int topY, int bottomY, Edge* leftEdge, Edge* rightEdge)
{
    for (int y = topY; y < bottomY; y++)
    {
        int startX = std::floor(leftEdge->getX() + 0.5);
        int endX = std::floor(rightEdge->getX() - 0.5);
        
        // Scanline Z interpolants
        double sliceZStep = (rightEdge->getZ() - leftEdge->getZ()) / (rightEdge->getX() - leftEdge->getX());
        double sliceZ = leftEdge->getZ() + ((std::ceil(leftEdge->getX() - 0.5) + 0.5) - leftEdge->getX()) * sliceZStep;
        
        double sliceUStep = (rightEdge->getU() - leftEdge->getU()) / (rightEdge->getX() - leftEdge->getX());
        double sliceU = leftEdge->getU() + ((std::ceil(leftEdge->getX() - 0.5) + 0.5) - leftEdge->getX()) * sliceUStep;
        
        double sliceVStep = (rightEdge->getV() - leftEdge->getV()) / (rightEdge->getX() - leftEdge->getX());
        double sliceV = leftEdge->getV() + ((std::ceil(leftEdge->getX() - 0.5) + 0.5) - leftEdge->getX()) * sliceVStep;
        
        for (int x = startX; x <= endX; x++)
        {
            double z = 1.0 / sliceZ;
            double u = sliceU / sliceZ;
            double v = sliceV / sliceZ;

            int index = y * viewSize.x + x;
            
            if (x >= 0 && x < viewSize.x && y >= 0 && y < viewSize.y)
            {
                if (z < zBuffer[index]) {
                    double k = z * 50;
                    if (k < 0) {
                        k = 0;
                    }
                    if (k > 255) {
                        k = 255;
                    }
                    
                    sf::Color texColor(texture.getPixel(std::floor(u * texture.getSize().x), std::floor(v * texture.getSize().y)));
                    //sf::Color fillColor(k, 0, 255);
                    putPixel(x, y, texColor);
                    zBuffer[index] = z;
                }
            }
            
            sliceZ += sliceZStep;
            sliceU += sliceUStep;
            sliceV += sliceVStep;
        }
        
        leftEdge->step();
        rightEdge->step();
    }
}

bool Camera::triangleInFrustum(Vertex* top, Vertex* mid, Vertex* bot)
{
    // Near plane test
    if (top->getCameraPosition().z > nearPlane && mid->getCameraPosition().z > nearPlane && bot->getCameraPosition().z > nearPlane)
    {
        // Right plane test
        if (top->getCameraPosition().x <= frustumStep.x * top->getCameraPosition().z &&
            mid->getCameraPosition().x <= frustumStep.x * mid->getCameraPosition().z &&
            bot->getCameraPosition().x <= frustumStep.x * bot->getCameraPosition().z)
        {
            // Left plane test
            if (top->getCameraPosition().x >= -frustumStep.x * top->getCameraPosition().z &&
                mid->getCameraPosition().x >= -frustumStep.x * mid->getCameraPosition().z &&
                bot->getCameraPosition().x >= -frustumStep.x * bot->getCameraPosition().z)
            {
                // Top plane test
                if (top->getCameraPosition().y <= frustumStep.y * top->getCameraPosition().z &&
                    mid->getCameraPosition().y <= frustumStep.y * mid->getCameraPosition().z &&
                    bot->getCameraPosition().y <= frustumStep.y * bot->getCameraPosition().z)
                {
                    // Bottom plane test
                    if (top->getCameraPosition().y >= -frustumStep.y * top->getCameraPosition().z &&
                        mid->getCameraPosition().y >= -frustumStep.y * mid->getCameraPosition().z &&
                        bot->getCameraPosition().y >= -frustumStep.y * bot->getCameraPosition().z)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
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

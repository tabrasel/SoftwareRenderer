#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <array>
#include "Polygon.hpp"
#include "Vertex.hpp"
#include "Scene.hpp"
#include "Edge.hpp"

class Camera
{
    private:
        sf::Vector3f position, angle;
        sf::Vector3f forward, sideways, upward;
        sf::Uint8* pixels;
        double* zBuffer;
        sf::Texture view;
        sf::Vector2f viewSize;
        double nearPlane;
        sf::Vector2i lastMousePos;
    
    public:
        Camera();
        ~Camera();
        void update();
        void viewScene(Scene& scene);
        void clearView();
        void drawTriangleHalf(int topY, int bottomY, Edge* leftEdge, Edge* rightEdge);
        void putPixel(int x, int y, sf::Color& color);
        bool polygonLeftHanded(Vertex& top, Vertex& v2, Vertex& v3);
        sf::Vector3f& getPosition();
        sf::Uint8* getPixels();
};

#endif

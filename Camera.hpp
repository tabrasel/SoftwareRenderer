#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <array>
#include "Polygon.hpp"
#include "Vertex.hpp"
#include "Scene.hpp"

class Camera
{
    private:
        sf::Vector3f position;
        sf::Uint8* pixels;
        sf::Texture view;
        sf::Vector2f viewSize;
        Polygon triangle;
        double nearPlane;
    
    public:
        Camera();
        void update();
        void viewScene(Scene& scene);
        void clearView();
        void putPixel(int x, int y, sf::Color& color);
        bool polygonLeftHanded(Vertex& top, Vertex& v2, Vertex& v3);
        sf::Vector3f& getPosition();
        sf::Uint8* getPixels();
};

#endif

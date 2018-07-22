#ifndef Quaternion_hpp
#define Quaternion_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

class Quaternion
{
    private:
        double x, y, z, w;
    
    public:
        Quaternion(sf::Vector3f& axis, double radians);
        //sf::Vector3f* rotateVector(sf::Vector3f& v);
        void rotateVector(sf::Vector3f& v);
};

#endif

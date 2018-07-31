#include "Quaternion.hpp"
#include <cmath>

Quaternion::Quaternion(sf::Vector3f& axis, double radians)
{
    w = std::cos(radians / 2);
    x = axis.x * std::sin(radians / 2);
    y = axis.y * std::sin(radians / 2);
    z = axis.z * std::sin(radians / 2);
}

void Quaternion::rotateVector(sf::Vector3f& v)
{
    sf::Vector3f result = sf::Vector3f(v.x, v.y, v.z);
    result.x = v.x * (x * x + w * w - y * y - z * z) + v.y * (2 * x * y - 2 * w * z) + v.z * (2 * x * z + 2 * w * y);
    result.y = v.x * (2 * w * z + 2 * x * y) + v.y * (w * w - x * x + y * y - z * z)+ v.z * (-2 * w * x + 2 * y * z);
    result.z = v.x * (-2 * w * y + 2 * x * z) + v.y * (2 * w * x + 2 * y * z) + v.z * (w * w - x * x - y * y + z * z);
    v.x = result.x;
    v.y = result.y;
    v.z = result.z;
}

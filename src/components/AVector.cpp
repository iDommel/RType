/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** AVector
*/

#include <cmath>
#include "AVector.hpp"

namespace ecs
{
    AVector::AVector(Component::Type type, float x, float y, float z) : Component(type), x(x), y(y), z(z)
    {
    }

    AVector::AVector(const AVector &other) : Component(other.getType()), x(other.x), y(other.y), z(other.z)
    {
    }

    AVector::AVector(const AVector &&other) : Component(other.getType()), x(other.x), y(other.y), z(other.z)
    {
    }

    float AVector::getDistance2D(const AVector &a, const AVector &b)
    {
        return std::sqrt(std::pow(b.x - a.x, 2.0f) + pow(b.y - a.y, 2.0f));
    }

    void AVector::operator=(const AVector &other)
    {
        _type = other.getType();
        x = other.x;
        y = other.y;
        z = other.z;
    }

    Vector2 AVector::getVector2() const
    {
        return {x, y};
    }

    std::tuple<float, float, float> AVector::getVector() const
    {
        return std::make_tuple(x, y, z);
    }

    void AVector::setVector(float newX, float newY, float newZ)
    {
        x = newX;
        y = newY;
        z = newZ;
    }

    AVector AVector::operator+(const AVector &other) const
    {
        return AVector(other.getType(), x + other.x, y + other.y, z + other.z);
    }

    AVector AVector::operator-(const AVector &other) const
    {
        return AVector(other.getType(), x - other.x, y - other.y, z - other.z);
    }

    AVector AVector::operator*(float scalar) const
    {
        return AVector(_type, x * scalar, y * scalar, z * scalar);
    }

}

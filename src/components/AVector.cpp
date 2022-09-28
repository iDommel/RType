/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** AVector
*/

#include "AVector.hpp"

namespace indie
{
    AVector::AVector(Component::Type type, float x, float y, float z):
    Component(type), x(x), y(y), z(z)
    {
    }

    AVector::AVector(const AVector &other):
    Component(other.getType()), x(other.x), y(other.y), z(other.z)
    {
    }

    AVector::AVector(const AVector &&other):
    Component(other.getType()), x(other.x), y(other.y), z(other.z)
    {
    }

    void AVector::operator=(const AVector &other)
    {
        _type = other.getType();
        x = other.x;
        y = other.y;
        z = other.z;
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

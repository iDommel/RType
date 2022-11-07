/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** AVector
*/

#ifndef AVECTOR_HPP_
#define AVECTOR_HPP_

#include <tuple>
#include "raylib.h"
#include "Component.hpp"

namespace ecs
{
    class AVector : public Component
    {
    public:
        AVector(Component::Type type, float x, float y, float z = 0);
        AVector(const AVector &other);
        AVector(const AVector &&other);
        virtual void operator=(const AVector &other);
        virtual std::tuple<float, float, float> getVector() const;
        virtual void setVector(float newX, float newY, float newZ);
        virtual AVector operator+(const AVector &other) const;
        virtual AVector operator-(const AVector &other) const;
        virtual AVector operator*(float scalar) const;
        virtual Vector2 getVector2() const;
        float x;
        float y;
        float z;
    };
}

#endif /* !AVECTOR_HPP_ */

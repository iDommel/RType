/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Sphere.cpp
*/

#include "raylib.h"

#include "Sphere.hpp"

namespace indie
{
    Sphere::Sphere(float radius, Color color) : Component(Type::SPHERE), _radius(radius), _color(color)
    {
        _isInitialized = true;
    }

    Color &Sphere::getColor()
    {
        return _color;
    }

    float Sphere::getRadius()
    {
        return _radius;
    }
}
/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Cube.cpp
*/

#include "raylib.h"

#include "Cube.hpp"

namespace indie
{
    Cube::Cube(Vector3 size, Color color) : Component(Type::CUBE), _size(size), _color(color)
    {
        _isInitialized = true;
    }

    Color &Cube::getColor()
    {
        return _color;
    }

    Vector3 &Cube::getSize()
    {
        return _size;
    }
}
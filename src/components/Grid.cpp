/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Grid.cpp
*/

#include "Grid.hpp"

#include "raylib.h"

namespace indie
{
    Grid::Grid(int slices, float spacing) : Component(Type::GRID), _slices(slices), _spacing(spacing)
    {
        _isInitialized = true;
    }

    int &Grid::getSlices()
    {
        return _slices;
    }

    float Grid::getSpacing()
    {
        return _spacing;
    }
}
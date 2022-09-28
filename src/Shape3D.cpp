/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Shape3D
*/

#include "raylib.h"

#include "Shape3D.hpp"

namespace indie {
Shape3D::Shape3D()
{
}

Shape3D::~Shape3D()
{
}

void Shape3D::drawSphere(Vector3 centerPos, float radius, Color color)
{
    ::DrawSphere(centerPos, radius, color);
}

void Shape3D::drawCube(Vector3 position, Vector3 size, Color color)
{
    DrawCubeV(position, size, color);
}

void Shape3D::drawGrid(int slices, float spacing)
{
    ::DrawGrid(slices, spacing);
}
}
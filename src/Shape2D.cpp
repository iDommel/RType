/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Shape2D
*/

#include "raylib.h"

#include "Shape2D.hpp"

namespace indie
{

    Shape2D::Shape2D()
    {
    }

    Shape2D::~Shape2D()
    {
    }

    void Shape2D::drawCircle(int centerX, int centerY, float radius, Color color)
    {
        DrawCircle(centerX, centerY, radius, color);
    }

    void Shape2D::drawRectangle(int posX, int posY, int width, int height, Color color)
    {
        DrawRectangle(posX, posY, width, height, color);
    }
}
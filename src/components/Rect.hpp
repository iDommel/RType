/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Sprite.hpp
*/

#ifndef RECT_HPP
#define RECT_HPP

#include <string>

#include "Component.hpp"

namespace ecs
{

    class Rect : public Component
    {
    public:
        Rect(float left = 0, float top = 0, float width = 0, float height = 0)
            : Component(Type::RECT), left(left), top(top), width(width), height(height)
        {
            _isInitialized = true;
        }

        /**
         * @brief Check if the given point is inside the rect
         * 
         * @param x X coordinate of the point
         * @param y Y coordinate of the point
         * @return true If the point is inside the rect
         */
        bool contains(float x, float y)
        {
            return (x >= left && x <= width && y >= top && y <= height);
        }

        float left;
        float top;
        float width;
        float height;
    };

}

#endif /* !RECT_HPP */
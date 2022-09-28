/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Sphere.hpp
*/

#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "Component.hpp"

struct Color;

namespace indie
{
    class Sphere : public Component
    {
    public:
        /**
         * @brief Construct a new Sphere object
         * @param radius radius of the sphere
         * @param color color of the sphere
         */
        Sphere(float radius, Color color);

        /// @brief Returns the color of the sphere
        Color &getColor();
        /// @brief Rerturns the radius of the sphere
        float getRadius();

    private:
        float _radius;
        Color _color;
    };
}

#endif /* !SPHERE_HPP_ */
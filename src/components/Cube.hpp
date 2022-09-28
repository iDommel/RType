/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Cube.hpp
*/

#ifndef Cube_HPP_
#define Cube_HPP_

#include "Component.hpp"

struct Vector3;
struct Color;

namespace indie
{
    class Cube : public Component
    {
    public:
        /**
         * @brief Construct a new Cube object
         * @param size size of the Cube
         * @param color color of the Cube
         */
        Cube(Vector3 size, Color color);

        /// @brief Returns the color of the Cube
        Color &getColor();
        /// @brief Rerturns the size of the Cube
        Vector3 &getSize();

    private:
        Vector3 _size;
        Color _color;
    };
}

#endif /* !Cube_HPP_ */
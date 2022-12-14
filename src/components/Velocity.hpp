/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Velocity
*/

#ifndef VELOCITY_HPP_
#define VELOCITY_HPP_

#include "AVector.hpp"

namespace ecs
{
    class Velocity : public AVector
    {
    public:
        Velocity(float x, float y, float z = 0);
    };
}

#endif /* !VELOCITY_HPP_ */

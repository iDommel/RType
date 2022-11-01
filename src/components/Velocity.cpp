/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Velocity
*/

#include "Velocity.hpp"

namespace ecs
{
    Velocity::Velocity(float x, float y, float z) : AVector(Component::Type::VELOCITY, x, y, z)
    {
    }

}

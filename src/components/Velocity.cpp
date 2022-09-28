/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Velocity
*/

#include "Velocity.hpp"

namespace indie
{
    Velocity::Velocity(float x, float y, float z):
    AVector(Component::Type::VELOCITY, x, y, z)
    {
    }

}

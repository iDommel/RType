/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Position
*/

#include "Position.hpp"

namespace indie
{
    Position::Position(float x, float y, float z):
    AVector(Component::Type::POSITION, x, y, z)
    {
        _isInitialized = true;
    }

    Position::Position(const Position &other):
        AVector(Component::Type::POSITION, other.x, other.y, other.z)
    {
        _isInitialized = true;
    }

    Position::Position(const Position &&other):
    AVector(Component::Type::POSITION, other.x, other.y, other.z)
    {
        _isInitialized = true;
    }

    void Position::operator=(const AVector &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    void Position::operator=(const Position &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    Position::Position(const AVector &other):
    AVector(Component::Type::POSITION, other.x, other.y, other.z)
    {
        _isInitialized = true;
    }
}

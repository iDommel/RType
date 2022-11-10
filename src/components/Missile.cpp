/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Missile
*/

#include "Missile.hpp"

namespace ecs
{
    Missile::Missile(MissileType type) : Component(Type::MISSILE), _missileType(type)
    {
        _isInitialized = true;
    }

    Missile::MissileType Missile::getMissileType() const
    {
        return _missileType;
    }
}  // namespace ecs

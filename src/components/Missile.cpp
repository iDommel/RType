/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Missile
*/

#include "Missile.hpp"

namespace ecs
{
    Missile::Missile(MissileType type, bool enemy) : Component(Type::MISSILE), _missileType(type), _enemy(enemy)
    {
        _isInitialized = true;
    }

    Missile::MissileType Missile::getMissileType() const
    {
        return _missileType;
    }

    bool Missile::isEnemy() const { return _enemy; }
}  // namespace ecs

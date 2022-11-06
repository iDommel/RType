/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Missile.hpp
*/

#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "Component.hpp"

namespace ecs
{
    class Missile : public Component
    {
    public:
        enum class MissileType {
            PL_SIMPLE,
            PL_CONDENSED,
            EN
        };

        Missile(MissileType type) : Component(Type::MISSILE), _missileType(type)
        {
            _isInitialized = true;
        }

    private:
        MissileType _missileType;
    };
}

#endif /* MISSILE_HPP */
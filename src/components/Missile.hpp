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
        enum class MissileType : qint8 {
            P_SIMPLE,
            P_CONDENSED,
            E_SINUSOIDAL,
            E_CLASSIC,
            HOMING_MISSILE, /// Separation for missile type: after missile have trajectory generated toward an entity
            E_HOMING_MISSILE,
            NB_MISSILE /// End of missile types
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
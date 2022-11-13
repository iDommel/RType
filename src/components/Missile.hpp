/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Missile.hpp
*/

#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "Component.hpp"
#include <QtGlobal>

namespace ecs
{
    class Missile : public Component
    {
    public:
        enum class MissileType : qint8 {
            P_SIMPLE,
            P_CONDENSED,
            E_RED3,
            E_RED2,
            E_BROWN1,
            E_BROWN2,
            E_BROWN4,
            HOMING_MISSILE, /// Separation for missile type: after missile have trajectory generated toward an entity
            E_HOMING_RED1,
            E_HOMING_RED4,
            E_HOMING_RED5,
            E_HOMING_BROWN3,
            E_HOMING_BROWN5,
            E_HOMING_GREEN1,
            E_HOMING_GREEN2,
            E_HOMING_GREEN3,
            E_HOMING_GREEN4,
            E_HOMING_GREEN5,
            NB_MISSILE /// End of missile types
        };
        Missile(MissileType type);

        MissileType getMissileType() const;

    private:
        MissileType _missileType;
    };
}

#endif /* MISSILE_HPP */
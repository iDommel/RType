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
            EN,
            NB
        };
        Missile(MissileType type);

        MissileType getMissileType() const;

    private:
        MissileType _missileType;
    };
}

#endif /* MISSILE_HPP */
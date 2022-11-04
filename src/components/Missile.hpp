/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Missile.hpp
*/

#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "Component.hpp"

#define MISSILE_DAMAGE  1.0f

namespace ecs
{
    class Missile : public Component
    {
    public:
        Missile(float damage) : Component(Type::MISSILE), _damage(damage)
        {
            _isInitialized = true;
        }

    private:
        float _damage = .0f;
    };
}

#endif /* MISSILE_HPP */
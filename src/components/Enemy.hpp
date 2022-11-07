/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Enemy.hpp
*/

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Component.hpp"

namespace ecs {
    class Enemy : public Component
    {
    public:
        enum class EnemyType : quint8 {
            SCOUT = 1,
            FIGHTER,
            TORPEDO,
            FRIGATE,
            NB
        };

        Enemy(EnemyType enemyType) : Component(Type::ENEMY), _enemyType(enemyType)
        {
            _isInitialized = true;
        };

        Enemy(quint8 enemyType) : Component(Type::ENEMY)
        {
            _isInitialized = true;
            _enemyType = EnemyType(enemyType);
        };

        EnemyType getEnemyType() const { return _enemyType; }

    private:
        EnemyType _enemyType;
    };
}

#endif /* ENEMY_HPP */
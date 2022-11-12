/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Enemy.cpp
*/

#include "GameSystem.hpp"

namespace ecs
{

    Enemy::Enemy(EnemyType enemyType, Missile::MissileType missileType, int shootCooldown, uint8_t missileNb)
        : Component(Type::ENEMY), _enemyType(enemyType), _missile(missileType), _missileNb(missileNb), _shootCooldown(std::chrono::milliseconds(shootCooldown))
    {
        _isInitialized = true;
        _shootTimer.setSingleShot(true);
        _salvoTimer.setSingleShot(true);
    }

    Enemy::Enemy(quint8 enemyType, Missile::MissileType missileType, int shootCooldown, uint8_t missileNb)
    : Component(Type::ENEMY)
    {
        _isInitialized = true;
        _enemyType = EnemyType(enemyType);
        _missile = missileType;
        _shootCooldown = std::chrono::milliseconds(shootCooldown);
        _shootTimer.setSingleShot(true);
        _salvoTimer.setSingleShot(true);
        _missileNb = missileNb;
    }

    std::shared_ptr<IEntity> Enemy::lootBonus(Position &pos)
    {
        if (_enemyType != EnemyType::FRIGATE)
            return nullptr;
        return GameSystem::createBonus(QUuid::createUuid(), pos);
    }

}
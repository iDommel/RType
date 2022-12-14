/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Enemy.hpp
*/

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Component.hpp"
#include "Missile.hpp"
#include "Bonus.hpp"

namespace ecs {
    class Enemy : public Component
    {
    public:
        enum class EnemyType : quint8 {
            REDSCOUT = 1,
            REDFIGHTER,
            REDTORPEDO,
            REDFRIGATE,
            REDTURRET,
            GREENSCOUT,
            GREENFIGHTER,
            GREENTORPEDO,
            GREENFRIGATE,
            GREENTURRET,
            BROWNSCOUT,
            BROWNFIGHTER,
            BROWNTORPEDO,
            BROWNFRIGATE,
            BROWNTURRET,
            NB
        };

        Enemy(EnemyType enemyType, Missile::MissileType missileType, int shootCooldown, uint8_t missileNb = 1);

        Enemy(quint8 enemyType, Missile::MissileType missileType, int shootCooldown, uint8_t missileNb = 1);

        EnemyType getEnemyType() const { return _enemyType; }

        void startShootTimer()
        {
            _shootTimer.start(_shootCooldown);
        };

        void startSalvoTimer()
        {
            _salvoTimer.start(std::chrono::milliseconds(200));
        }

        bool isShootTime() const
        {
            return (_shootTimer.remainingTime() <= 0);
        }

        bool salvoTime() const
        {
            return (_salvoTimer.remainingTime() <= 0);
        }

        void setShooting(bool value) { _shooting = value; }
        void reverseShooting() { _shooting = (!_shooting); }
        bool isShooting() const { return _shooting; }

        uint8_t &getSalvo() { return _salvo; }
        uint8_t getNbMissile() const { return _missileNb; }

        Missile::MissileType getMissileType() const { return _missile; }

        std::shared_ptr<IEntity> lootBonus(Position &pos);

    private:
        EnemyType _enemyType;
        Missile::MissileType _missile;
        uint8_t _missileNb = 1;
        uint8_t _salvo = 0;
        QTimer _salvoTimer;
        QTimer _shootTimer;
        std::chrono::milliseconds _shootCooldown;
        bool _shooting = false;
    };
}

#endif /* ENEMY_HPP */
/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Boss.hpp
*/

#ifndef BOSS_HPP
#define BOSS_HPP

#include "Missile.hpp"
#include "Component.hpp"
#include <chrono>
#include <map>
#include <QTimer>

namespace ecs {

    class IEntity;

    class Boss : public Component
    {
    public:
        enum class BossType {
            BOSS_1,
            BOSS_2,
            BOSS_3
        };

        Boss(BossType type);

        BossType getBossType() const { return _bossType; }

        /// @brief Adds a missile salvo to the boss
        /// @param missileType Type of the missile
        /// @param cooldown Time in ms between two salvo
        /// @param nb Number of missile in a salvo
        /// @param salvoCooldown Time in ms between two missile in a salvo
        Boss &addMissileSalvo(Missile::MissileType missileType, uint32_t cooldown, uint8_t nb, uint32_t salvoCooldown);

        /// @return Returns True if a salvo is in progress, False otherwise.
        bool isShooting() const;
        void setShooting(bool value);

        uint8_t getSalvoCounter() const;
        void setSalvoCounter(uint8_t counter);

        /// @return Returns True if timer for shooting a salvo has timed out, False otherwise.
        bool isShootTime() const;
        /// @return Returns True if timer for shooting a missile in a salvo has timed out, False otherwise.
        bool isSalvoTime() const;

        void startShootTimer(Missile::MissileType type);
        void startSalvoTimer();

        /// @param manager Scene manager
        /// @param entity Boss entity
        /// @param nextType Next missile type to shoot if salvo is finished
        /// @return Returns the new missile entity or nullptr if not shooted
        std::shared_ptr<IEntity> shoot(SceneManager &manager, std::shared_ptr<IEntity> entity, Missile::MissileType nextType);

    private:
        BossType _bossType;
        std::map<Missile::MissileType, std::chrono::milliseconds> _missiles;
        std::map<Missile::MissileType, std::pair<uint8_t, std::chrono::milliseconds>> _missilesSalvo;
        QTimer _shootTimer;
        QTimer _salvoTimer;
        Missile::MissileType _currMissile = Missile::MissileType::NB_MISSILE;
        uint8_t _salvoCounter = 0;
        bool _shooting = false;
    };

}

#endif /* BOSS_HPP */
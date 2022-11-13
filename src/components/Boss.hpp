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
#include "SceneManager.hpp"
#include <chrono>
#include <map>
#include <QTimer>

namespace ecs {

    class IEntity;

    class Boss : public Component
    {
    public:
        enum class BossType : quint8 {
            BOSS_1,
            BOSS_2,
            BOSS_3,
            NB
        };

        /// @param type Type of the Boss
        /// @param tank Number of missile the boss can tank
        Boss(BossType type, uint32_t tank);

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
        bool isShootTime(Missile::MissileType type) const;
        /// @return Returns True if timer for shooting a missile in a salvo has timed out, False otherwise.
        bool isSalvoTime(Missile::MissileType type) const;

        /// @return Returns a reference to the intern number of tanked missile
        uint32_t &getTankedMissile();
        uint32_t getTankMax() const;

        /// @param manager Scene manager
        /// @param entity Boss entity
        /// @param nextType Next missile type to shoot if salvo is finished
        /// @return Returns the new missile entity or nullptr if not shooted
        std::shared_ptr<IEntity> shoot(SceneManager &manager, std::shared_ptr<IEntity> entity);

    private:
        BossType _bossType;
        std::map<Missile::MissileType, std::chrono::milliseconds> _missiles;
        std::map<Missile::MissileType, std::pair<uint8_t, std::chrono::milliseconds>> _missilesSalvo;
        std::map<Missile::MissileType, QTimer> _shootTimers;
        std::map<Missile::MissileType, QTimer> _salvoTimers;
        // QTimer _shootTimer;
        // QTimer _salvoTimer;
        uint8_t _salvoCounter = 0;
        uint32_t _tank;
        uint32_t _tankCounter = 0;
        bool _shooting = false;
        Missile::MissileType _currMissile = Missile::MissileType::NB_MISSILE;
    };

}

#endif /* BOSS_HPP */
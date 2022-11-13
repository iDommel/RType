/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Boss.cpp
*/

#include "Boss.hpp"
#include "GameSystem.hpp"

namespace ecs
{

    Boss::Boss(BossType type, uint32_t tank) : Component(Type::BOSS), _bossType(type), _tank(tank)
    {
        _isInitialized = true;
        _shootTimer.setSingleShot(true);
        _salvoTimer.setSingleShot(true);
    }

    Boss &Boss::addMissileSalvo(Missile::MissileType missileType, uint32_t cooldown, uint8_t nb, uint32_t salvoCooldown)
    {
        if (_currMissile == Missile::MissileType::NB_MISSILE)
            _currMissile = missileType;
        _missiles[missileType] = std::chrono::milliseconds(cooldown);
        _missilesSalvo[missileType] = std::make_pair(nb, std::chrono::milliseconds(salvoCooldown));
        return *this;
    }

    bool Boss::isShooting() const
    {
        return _shooting;
    }

    void Boss::setShooting(bool value)
    {
        _shooting = value;
    }

    uint8_t Boss::getSalvoCounter() const
    {
        return _salvoCounter;
    }

    void Boss::setSalvoCounter(uint8_t counter)
    {
        _salvoCounter = counter;
    }

    bool Boss::isShootTime() const
    {
        return (_shootTimer.remainingTime() <= 0);
    }

    bool Boss::isSalvoTime() const
    {
        return (_salvoTimer.remainingTime() <= 0);
    }

    void Boss::startShootTimer(Missile::MissileType type)
    {
        _shootTimer.start(_missiles[type]);
        _currMissile = type;
    }

    void Boss::startSalvoTimer()
    {
        if (_currMissile == Missile::MissileType::NB_MISSILE)
            return;
        _salvoTimer.start(_missilesSalvo[_currMissile].second);
    }

    uint32_t &Boss::getTankedMissile() { return _tankCounter; }

    uint32_t Boss::getTankMax() const { return _tank; }

    std::shared_ptr<IEntity> Boss::shoot(SceneManager &manager, std::shared_ptr<IEntity> entity, Missile::MissileType nextType)
    {
        QUuid id = QUuid::createUuid();
        auto pos = Component::castComponent<Position>((*entity)[IComponent::Type::POSITION]);

        if (isShootTime() && !_shooting) {
            GameSystem::createMissile(manager, id, *pos, _currMissile, IEntity::Tags::PLAYER);
            if (_missilesSalvo[_currMissile].first > 1) {
                _shooting = true;
                startSalvoTimer();
                _salvoCounter++;
            } else
                startShootTimer(nextType);
        } else if (isSalvoTime() && _shooting) {
            GameSystem::createMissile(manager, id, *pos, _currMissile, IEntity::Tags::PLAYER);
            if (++_salvoCounter >= _missilesSalvo[_currMissile].first) {
                _salvoCounter = 0;
                startShootTimer(nextType);
                _shooting = false;
            } else
                startSalvoTimer();
        }
        return manager.getCurrentScene().getEntityById(id);
    }

}
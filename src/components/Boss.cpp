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
    }

    Boss &Boss::addMissileSalvo(Missile::MissileType missileType, uint32_t cooldown, uint8_t nb, uint32_t salvoCooldown)
    {
        _missiles[missileType] = std::chrono::milliseconds(cooldown);
        _missilesSalvo[missileType] = std::make_pair(nb, std::chrono::milliseconds(salvoCooldown));
        _shootTimers[missileType].setSingleShot(true);
        _salvoTimers[missileType].setSingleShot(true);
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

    bool Boss::isShootTime(Missile::MissileType type) const
    {
        return (_shootTimers.at(type).remainingTime() <= 0);
    }

    bool Boss::isSalvoTime(Missile::MissileType type) const
    {
        return (_salvoTimers.at(type).remainingTime() <= 0);
    }

    // void Boss::startShootTimer(Missile::MissileType type)
    // {
    //     _shootTimer.start(_missiles[type]);
    //     _currMissile = type;
    // }

    // void Boss::startSalvoTimer()
    // {
    //     if (_currMissile == Missile::MissileType::NB_MISSILE)
    //         return;
    //     _salvoTimer.start(_missilesSalvo[_currMissile].second);
    // }

    uint32_t &Boss::getTankedMissile() { return _tankCounter; }

    uint32_t Boss::getTankMax() const { return _tank; }

    std::shared_ptr<IEntity> Boss::shoot(SceneManager &manager, std::shared_ptr<IEntity> entity)
    {
        QUuid id = QUuid::createUuid();
        auto posEntity = Component::castComponent<Position>((*entity)[IComponent::Type::POSITION]);
        Position pos(*posEntity);

        pos.y += SCALE / 2;
        if (_shooting && isSalvoTime(_currMissile)) {
            GameSystem::createMissile(manager, id, pos, _currMissile, IEntity::Tags::PLAYER);
            if (++_salvoCounter >= _missilesSalvo[_currMissile].first) {
                _salvoCounter = 0;
                _shootTimers[_currMissile].start(_missiles[_currMissile]);
                _shooting = false;
            } else
                _salvoTimers[_currMissile].start(_missilesSalvo[_currMissile].second);
        } else if (_shooting)
            return nullptr;
        else {
            for (auto &shootTimer : _shootTimers) {
                Missile::MissileType type = shootTimer.first;
                if (isShootTime(type) && !_shooting) {
                    GameSystem::createMissile(manager, id, pos, type, IEntity::Tags::PLAYER);
                    if (_missilesSalvo[type].first > 1) {
                        _shooting = true;
                        _salvoTimers[type].start(_missilesSalvo[type].second);
                        _salvoCounter++;
                        _currMissile = type;
                    } else
                        _shootTimers[type].start(_missiles[type]);
                    break;
                }
            }
        }
        return manager.getCurrentScene().getEntityById(id);
    }

}
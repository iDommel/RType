/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Player
*/
#include "raylib.h"

#include <functional>
#include <cmath>
#include <algorithm>

#include "Player.hpp"
#include "ButtonCallbacks.hpp"
#include "Entity.hpp"
#include "Position.hpp"
#include "String.hpp"
#include "Velocity.hpp"
#include "HitboxComponent.hpp"
#include "Bomb.hpp"
#include "SoundComponent.hpp"
#include "Bonus.hpp"
#include "Sphere.hpp"
#include "GameSystem.hpp"
#include "Model3D.hpp"
#include "Missile.hpp"
#include "Sprite.hpp"
#include "Trajectory.hpp"

namespace ecs
{
    const int Player::_defaultSpeed = 140;

    Player::Player(int id, int _up, int _down, int _left, int _right, int _bomb) : Component(Type::PLAYER), _id(id)
    {
        UP = GameSystem::getBinding(_up);
        DOWN = GameSystem::getBinding(_down);
        LEFT = GameSystem::getBinding(_left);
        RIGHT = GameSystem::getBinding(_right);
        BOMB = GameSystem::getBinding(_bomb);
        changeBomb = 0;
        changeDown = 0;
        changeUp = 0;
        changeLeft = 0;
        changeRight = 0;
        _blastPower = _defaultBlastPower;
        _speed = _defaultSpeed;
        _blastPower = _defaultBlastPower;
    }

    Player::~Player()
    {
    }

    void Player::handleBonus(const Bonus &bonus)
    {
        if (bonus.getBonusType() == Bonus::Type::SPEED)
            _speed += 10;
        else if (bonus.getBonusType() == Bonus::Type::POWER)
            _blastPower++;
    }

    void Player::moveRight(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        _isRight = true;
        move(vel);
    }

    void Player::stopRight(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        _isRight = false;
        move(vel);
    }

    void Player::moveLeft(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        _isLeft = true;
        move(vel);
    }

    void Player::stopLeft(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        _isLeft = false;
        move(vel);
    }

    void Player::moveUp(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        _isUp = true;
        move(vel);
    }

    void Player::moveHorizontal(SceneManager &, std::shared_ptr<IEntity> entity, float value)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        vel->x = (_speed * value);
    }

    void Player::moveVertical(SceneManager &, std::shared_ptr<IEntity> entity, float value)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        vel->z = (_speed * value);
    }

    void Player::stopUp(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        _isUp = false;
        move(vel);
    }

    void Player::moveDown(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        _isDown = true;
        move(vel);
    }

    void Player::stopDown(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);

        _isDown = false;
        move(vel);
    }

    void Player::move(std::shared_ptr<Velocity> vel)
    {
        vel->y = (_speed * _isDown) + (-_speed * _isUp);
        vel->x = (_speed * _isRight) + (-_speed * _isLeft);
    }

    int Player::getId() const
    {
        return _id;
    }

    int Player::getSpeed() const
    {
        return _speed;
    }

    void Player::startClock()
    {
        _shootTimer = QTime::currentTime();
    }

    QTime &Player::getShootTimer()
    {
        return _shootTimer;
    }

    int Player::getBlastPower() const
    {
        return _blastPower;
    }
    std::string Player::getUp()
    {
        return UP;
    }

    std::string Player::getDown()
    {
        return DOWN;
    }

    std::string Player::getLeft()
    {
        return LEFT;
    }

    std::string Player::getRight()
    {
        return RIGHT;
    }

    std::string Player::getBomb()
    {
        return BOMB;
    }

    int Player::getTagUp()
    {
        return GameSystem::getTag(UP);
    }

    int Player::getTagDown()
    {
        return GameSystem::getTag(DOWN);
    }

    int Player::getTagLeft()
    {
        return GameSystem::getTag(LEFT);
    }

    int Player::getTagRight()
    {
        return GameSystem::getTag(RIGHT);
    }

    int Player::getTagBomb()
    {
        return GameSystem::getTag(BOMB);
    }

    void Player::setUP(std::string _up)
    {
        UP = _up;
    }

    void Player::setDOWN(std::string _down)
    {
        DOWN = _down;
    }

    void Player::setLEFT(std::string _left)
    {
        LEFT = _left;
    }

    void Player::setRIGHT(std::string _right)
    {
        RIGHT = _right;
    }

    void Player::setBOMB(std::string _bomb)
    {
        BOMB = _bomb;
    }

    void Player::kill()
    {
        _isDead = true;
    }

    bool Player::isDead() const
    {
        return _isDead;
    }
}
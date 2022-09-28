/*
** EPITECH PROJECT, 2022
** indieStudio
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

namespace indie
{

    Player::Player(int id, int _up, int _down, int _left, int _right, int _bomb):
    Component(Type::PLAYER), _id(id)
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
        _nbBomb = _defaultNbBomb;
        _blastPower = _defaultBlastPower;
        _speed = _defaultSpeed;
        _blastPower = _defaultBlastPower;
    }

    Player::~Player()
    {
    }

    void Player::handleBonus(const Bonus &bonus)
    {
        if (bonus.getBonusType() == Bonus::Type::BOMB)
            _nbBomb++;
        else if (bonus.getBonusType() == Bonus::Type::SPEED)
            _speed += 10;
        else if (bonus.getBonusType() == Bonus::Type::POWER)
            _blastPower++;
    }

    void Player::moveRight(SceneManager &, std::shared_ptr<IEntity> entity, float)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        auto model = Component::castComponent<Model3D>((*entity)[Component::Type::MODEL]);

        model->setRotation(90.0f);
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
        auto model = Component::castComponent<Model3D>((*entity)[Component::Type::MODEL]);

        _isLeft = true;
        model->setRotation(270.0f);
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
        auto model = Component::castComponent<Model3D>((*entity)[Component::Type::MODEL]);

        model->setRotation(180.0f);
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
        auto model = Component::castComponent<Model3D>((*entity)[Component::Type::MODEL]);

        model->setRotation(0.0f);
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
        vel->z = (_speed * _isDown) + (-_speed * _isUp);
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

    int Player::getNbBomb() const
    {
        return _nbBomb;
    }

    void Player::setNbBomb(int newNbBomb)
    {
        _nbBomb = newNbBomb;
    }

    void Player::generateBomb(SceneManager &manager, std::shared_ptr<IEntity> entity)
    {
        int bombNb = _bombs.size();
    
        if (bombNb >= _nbBomb)
            return;

        std::shared_ptr<Entity> bomb = std::make_shared<Entity>();
        std::shared_ptr<Entity> timerSound = std::make_shared<Entity>();
        auto pos = Component::castComponent<Position>((*entity)[Component::Type::POSITION]);
        Vector3 size = {GAME_TILE_SIZE, GAME_TILE_SIZE, GAME_TILE_SIZE};
        Vector3 bPos = {std::roundf(pos->x / GAME_TILE_SIZE) * GAME_TILE_SIZE - GAME_TILE_SIZE / 2, pos->y, std::roundf(pos->z / GAME_TILE_SIZE) * GAME_TILE_SIZE - GAME_TILE_SIZE / 2};

        bomb->addComponent(std::make_shared<Bomb>(_blastPower))
            .addComponent(std::make_shared<Position>(std::roundf(pos->x / GAME_TILE_SIZE) * GAME_TILE_SIZE, pos->y, std::roundf(pos->z / GAME_TILE_SIZE) * GAME_TILE_SIZE))
            .addComponent(std::make_shared<Model3D>("assets/other_asset/water_bomb/water_bomb.obj", "assets/other_asset/water_bomb/water_bomb.png", 2.0f))
            .addComponent(std::make_shared<Hitbox>(CollideSystem::makeBBoxFromSizePos(size, bPos)));
        _bombs.push_back(bomb);
        timerSound->addComponent(std::make_shared<SoundComponent>("sound_det"));
        manager.getCurrentScene().addEntity(bomb).addEntity(timerSound);
    }

    void Player::updateBombsVec()
    {
        for (auto bomb = _bombs.begin(); bomb != _bombs.end();) {
            auto b = Component::castComponent<Bomb>((**bomb)[IComponent::Type::BOMB]);
            if (b->getTimer() <= 0)
                bomb = _bombs.erase(std::find(_bombs.begin(), _bombs.end(), *bomb));
            else
                bomb++;
        }
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
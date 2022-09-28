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

#include "AIPlayer.hpp"
#include "ButtonCallbacks.hpp"
#include "Entity.hpp"
#include "Position.hpp"
#include "Velocity.hpp"
#include "HitboxComponent.hpp"
#include "Bomb.hpp"
#include "Sphere.hpp"
#include "GameSystem.hpp"
#include "Bonus.hpp"
#include "Model3D.hpp"

namespace indie
{
    AIPlayer::AIPlayer(int id) : Component(Type::AI), _id(id)
    {
    }

    void AIPlayer::handleBonus(const Bonus &bonus)
    {
        if (bonus.getBonusType() == Bonus::Type::BOMB)
            _nbBombMax++;
        else if (bonus.getBonusType() == Bonus::Type::SPEED)
            _speed += 10;
        else if (bonus.getBonusType() == Bonus::Type::POWER)
            _blastPower++;
    }

    void AIPlayer::move(std::shared_ptr<Velocity> vel)
    {
        vel->z = (_speed * _isDown) + (-_speed * _isUp);
        vel->x = (_speed * _isRight) + (-_speed * _isLeft);
    }

    int AIPlayer::getId() const
    {
        return _id;
    }

    int AIPlayer::getSpeed() const
    {
        return _speed;
    }

    int AIPlayer::getNbBomb() const
    {
        return _nbBombMax;
    }

    void AIPlayer::setNbBomb(int newNbBomb)
    {
        _nbBombMax = newNbBomb;
    }

    void AIPlayer::setRadar(std::shared_ptr<IEntity> radar)
    {
        _radar = radar;
    }

    std::shared_ptr<IEntity> AIPlayer::getRadar() const
    {
        return _radar;
    }

    void AIPlayer::moveRight(std::shared_ptr<IEntity> entity)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        _isRight = true;
        move(vel);
    }

    void AIPlayer::stopRight(std::shared_ptr<IEntity> entity)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        _isRight = false;
        move(vel);
    }

    void AIPlayer::moveLeft(std::shared_ptr<IEntity> entity)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        _isLeft = true;
        move(vel);
    }

    void AIPlayer::stopLeft(std::shared_ptr<IEntity> entity)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        _isLeft = false;
        move(vel);
    }

    void AIPlayer::moveUp(std::shared_ptr<IEntity> entity)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        _isUp = true;
        move(vel);
    }

    void AIPlayer::stopUp(std::shared_ptr<IEntity> entity)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        _isUp = false;
        move(vel);
    }

    void AIPlayer::moveDown(std::shared_ptr<IEntity> entity)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        _isDown = true;
        move(vel);
    }

    void AIPlayer::stopDown(std::shared_ptr<IEntity> entity)
    {
        auto vel = Component::castComponent<Velocity>((*entity)[Component::Type::VELOCITY]);
        _isDown = false;
        move(vel);
    }

    void AIPlayer::generateBomb(SceneManager &manager, Vector3 &pos)
    {
        if (_bombs.size() >= _nbBombMax)
            return;

        std::shared_ptr<Entity> bomb = std::make_shared<Entity>();
        Vector3 size = {GAME_TILE_SIZE, GAME_TILE_SIZE, GAME_TILE_SIZE};
        Vector3 bPos = {pos.x - GAME_TILE_SIZE/2, pos.y, pos.z - GAME_TILE_SIZE/2};

        bomb->addComponent(std::make_shared<Bomb>(_blastPower))
            .addComponent(std::make_shared<Position>(pos.x, pos.y, pos.z))
            .addComponent(std::make_shared<Model3D>("assets/other_asset/water_bomb/water_bomb.obj", "assets/other_asset/water_bomb/water_bomb.png", 2.0f))
            .addComponent(std::make_shared<Hitbox>(CollideSystem::makeBBoxFromSizePos(size, bPos)));
        _bombs.push_back(bomb);
        manager.getCurrentScene().addEntity(bomb);
    }

    void AIPlayer::updateBombsVec()
    {
        for (auto bomb = _bombs.begin(); bomb != _bombs.end();) {
            auto b = Component::castComponent<Bomb>((**bomb)[IComponent::Type::BOMB]);
            if (b->getTimer() <= 0)
                bomb = _bombs.erase(std::find(_bombs.begin(), _bombs.end(), *bomb));
            else
                bomb++;
        }
    }
}

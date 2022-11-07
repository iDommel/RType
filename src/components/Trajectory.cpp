/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Trajectory
*/

#include <iostream>
#include "Trajectory.hpp"

namespace ecs
{
    Trajectory::Trajectory(std::function<float(float)> trajFuncX, std::function<float(float)> trajFuncY, std::shared_ptr<Position> origin) : Component(Type::TRAJECTORY), _trajFuncX(trajFuncX), _trajFuncY(trajFuncY)
    {
        _isInitialized = true;
        _timer = 0;
        _origin = std::make_shared<Position>(*origin);
    }

    // need to add speed of the entity
    void Trajectory::update(std::shared_ptr<Position> actualPos)
    {
        _timer++;
        actualPos->y = _origin->y + _trajFuncY(_timer);
        actualPos->x = _origin->x + _trajFuncX(_timer);
    }
}
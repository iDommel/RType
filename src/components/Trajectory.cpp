/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Trajectory
*/

#include "Trajectory.hpp"

namespace ecs
{
    Trajectory::Trajectory(std::function<int(int)> trajFunction, Position pos) : Component(Type::TRAJECTORY), _trajFunction(trajFunction), _origin(pos)
    {
        _isInitialized = true;
        _timer = 0;
    }

    // need to add speed of the entity
    void Trajectory::Update(std::shared_ptr<Position> actualPos)
    {
        _timer++;
        actualPos->y = _origin.y + _trajFunction(_timer);
        actualPos->x = _origin.x + _timer / 60;
    }
}
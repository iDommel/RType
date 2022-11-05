/*
** EPITECH PROJECT, 2022
** RType
** File description:
** Trajectory
*/

#ifndef TRAJECTORY_HPP_
#define TRAJECTORY_HPP_

#include <map>
#include <functional>
#include <cmath>
#include "Position.hpp"
#include "Component.hpp"

namespace ecs
{
    class Trajectory : public Component
    {
    public:
        /// @brief Construct a new trajectory depending a std function
        /// @param trajectoryFunctionX that define the trajectory to follow on X axes
        /// @param trajectoryFunctionY that define the trajectory to follow on Y axes
        /// @param the origin of the entity;
        Trajectory(std::function<float(float)> trajFuncX, std::function<float(float)> trajFuncY, std::shared_ptr<Position> origin);

        /// @brief Called at each update to make the entity move depending to the trajectory
        void update(std::shared_ptr<Position> actualPos);

    private:
        std::function<float(float)> _trajFuncX;
        std::function<float(float)> _trajFuncY;
        float _timer;
        std::shared_ptr<Position> _origin;
    };
}

#endif /* !TRAJECTORY_HPP_ */

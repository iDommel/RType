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
        /// @brief List of exemple trjectory functions. Y is the result, X the given value
        enum class TrajectoryType
        {
            /// @brief y = 0
            LINEAR,
            /// @brief y = x / tps
            TOPAFINE,
            /// @brief y = -x / tps
            BotAFINE,
            /// @brief y = log(x / tps)
            LOG,
            /// @brief y = -log(x / tps)
            NEGLOG,
            /// @brief y = syn(x / tps) * 10
            SINUSOIDAL,
            /// @brief y = -syn(x / tps) * 10
            NEGSINUSOIDAL
        };

        /// @brief Construct a new trajectory depending a std function
        /// @param trajectoryFunction that define the trajectory to follow
        Trajectory(std::function<int(int)> trajFunction, Position pos);
        /// @brief Called at each update to make the entity move depending to the trajectory
        void Update(std::shared_ptr<Position> actualPos);
        /// @brief Use TrajectoryType to call the matching std::function
        std::map<TrajectoryType, std::function<int(int)>> trajectoryList = {
            {LINEAR, [](int a)
             { return 0; }},
            {TOPAFINE, [](int a)
             { return a / 60; }},
            {BOTAFINE, [](int a)
             { return a / 60 * -1; }},
            {LOG, [](int a)
             { return std::log(a / 60); }},
            {NEGLOG, [](int a)
             { return std::log(a / 60) * -1; }},
            {SINUSOIDAL, [](int a)
             { return std::sin(a / 60) * 10; }},
            {NEGSINUSOIDAL, [](int a)
             { return std::sin(a / 60) * -10; }}};

    private:
        std::function<int(int)> _trajFunction;
        float _timer;
        Position _origin;
    };
}

#endif /* !TRAJECTORY_HPP_ */

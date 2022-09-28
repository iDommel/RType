/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Core.hpp
*/

#ifndef CORE_HPP
#define CORE_HPP

#include <map>
#include <iostream>

#include "systems/ISystem.hpp"
#include "SceneManager.hpp"

#define UPDATE_DELTA 17

namespace indie
{
    class Core
    {
    public:
        /**
         * @brief Types of systems: systems init and destroy calls are effectued by ascending order
         */
        enum class SystemType {
            GAME,
            EVENT,
            AUDIO,
            GRAPHIC,
            PARTICLE,
            /// sub-system of game system
            COLLIDE,
            /// sub-system of game system
            AI
        };

        Core();

        /// @brief Game loop
        void mainLoop();

        /**
         * @brief Call each loadEntity system function, set as addEntity callback
         * @param entity Entity to load
         */
        void loadEntity(std::shared_ptr<IEntity> entity);

        /**
         * @brief Call each unloadEntity system function, set as removeEntity callback
         * @param entity Entity to unload
         */
        void unloadEntity(std::shared_ptr<IEntity> entity);

    private:
        std::map<SystemType, std::unique_ptr<ISystem>> _systems;
        SceneManager _sceneManager;
        bool _end = false;
    };
}

#endif /* CORE_HPP */

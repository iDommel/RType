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
#include <QtCore>

#include "systems/ISystem.hpp"
#include "SceneManager.hpp"

#define UPDATE_DELTA 17

namespace ecs
{
    class Core : public QCoreApplication
    {

        Q_OBJECT

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

        /// @brief Construct a core with enabled systems
        /// @param ActiveSystems systems to enable
        Core(int ac, char **av, std::vector<SystemType> ActiveSystems);

        /// @brief init systems & launch game loop
        void run();

        /**
         * @brief Call each onEntityAdded system function, set as addEntity callback
         * @param entity Entity to load
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity);

        /**
         * @brief Call each onEntityRemoved system function, set as removeEntity callback
         * @param entity Entity to unload
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity);

    private slots:
        void loop();

    signals:
        void doLoop();

    private:
        void systemUpdate(SystemType, SceneManager &, int64_t);
        std::map<SystemType, std::unique_ptr<ISystem>> _systems;
        SceneManager _sceneManager;
        bool _end = false;
        std::chrono::_V2::system_clock::time_point _clock;
    };
}

#endif /* CORE_HPP */

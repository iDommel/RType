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
#include "ANetworkSystem.hpp"

#define UPDATE_DELTA        17
#define NB_CLIENTS_MAX      4

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
            NETWORK,
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
        Core(int ac, char **av, std::vector<SystemType> ActiveSystems, NetworkRole role = NetworkRole::UNDEFINED);
        ~Core();

        /// @brief Connect EventSystem & ANetworkSystem for networked events
        /// @throw Needs to be called before run() & both systems needs to be active
        void setEventNetwork();

        /// @brief init systems & launch game loop
        void run();

        /**
         * @brief Call each onEntityAdded system function, set as addEntity callback
         * @param entity Entity to load
         * @param scene Scene to add entity into
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity, SceneType scene);

        /**
         * @brief Call each onEntityRemoved system function, set as removeEntity callback
         * @param entity Entity to unload
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity);

        /// @brief Network role: Client or Server
        static NetworkRole networkRole;

    private slots:
        void loop();
        void onChangeScene(SceneType scene);

    signals:
        void doLoop();
        void exitApp();

    private:
        void systemUpdate(SystemType, SceneManager &, int64_t);
        std::map<SystemType, ISystem *> _systems;
        SceneManager _sceneManager;
        bool _end = false;
        std::chrono::_V2::system_clock::time_point _clock;
        bool _running = false;
    };
}

#endif /* CORE_HPP */

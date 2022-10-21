/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Core.cpp
*/

#include "raylib.h"

#include <chrono>
#include <thread>

#include "Core.hpp"
#include "systems/AudioSystem.hpp"
#include "systems/EventSystem.hpp"
#include "systems/GameSystem.hpp"
#include "systems/GraphicSystem.hpp"
#include "systems/CollideSystem.hpp"
#include "systems/ParticlesSystem.hpp"
#include "systems/NetworkClientSystem.hpp"
#include "systems/NetworkServerSystem.hpp"

namespace ecs
{

    Core::Core(int ac, char **av, std::vector<SystemType> activeSystems, NetworkRole role) : QCoreApplication(ac, av)
    {
        networkRole = role;

        // Connect signal doLoop to loop function
        connect(this, &Core::doLoop, this, &Core::loop, Qt::QueuedConnection);
        connect(this, &Core::exitApp, this, &QCoreApplication::quit, Qt::QueuedConnection);

        for (auto &system : activeSystems) {
            switch (system) {
            case SystemType::GAME:
                _systems[system] = new GameSystem();
                break;
            case SystemType::EVENT:
                _systems[system] = new EventSystem();
                break;
            case SystemType::AUDIO:
                _systems[system] = new AudioSystem();
                break;
            case SystemType::GRAPHIC:
                _systems[system] = new GraphicSystem();
                break;
            case SystemType::PARTICLE:
                _systems[system] = new ParticlesSystem();
                break;
            case SystemType::NETWORK:
                if (role == NetworkRole::CLIENT) {
                    auto client = new NetworkClientSystem();
                    _systems[system] = client;
                    connect(this, &QCoreApplication::aboutToQuit, client, &NetworkClientSystem::destroy);
                } else if (role == NetworkRole::SERVER)
                    _systems[system] = new NetworkServerSystem();
                break;
            default:
                break;
            }
        }

        // if (networkRole == NetworkRole::CLIENT) {
        //     auto netSys = dynamic_cast<ANetworkSystem *>(_systems[SystemType::NETWORK]);
        //     connect(netSys, &ANetworkSystem::clientConnection, this, &Core::onClientConnection);
        // }
    }

    Core::~Core() {}

    void Core::run()
    {
        _running = true;
        _clock = std::chrono::high_resolution_clock::now();

        for (auto &system : _systems)
            system.second->init(_sceneManager);
        _sceneManager.setAddEntityCallback(std::bind(&Core::onEntityAdded, this, std::placeholders::_1));
        _sceneManager.setRemoveEntityCallback(std::bind(&Core::onEntityRemoved, this, std::placeholders::_1));

        emit doLoop();
    }

    void Core::loop()
    {
        auto time = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(time - _clock).count();

        if (deltaTime < UPDATE_DELTA) {
            std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_DELTA - deltaTime));
        } else {
            systemUpdate(SystemType::EVENT, _sceneManager, deltaTime);
            systemUpdate(SystemType::GAME, _sceneManager, deltaTime);
            systemUpdate(SystemType::NETWORK, _sceneManager, deltaTime);
            systemUpdate(SystemType::AUDIO, _sceneManager, deltaTime);
            systemUpdate(SystemType::PARTICLE, _sceneManager, deltaTime);
            systemUpdate(SystemType::GRAPHIC, _sceneManager, deltaTime);
            _clock = time;
        }
        if (!_sceneManager.getShouldClose())
            emit doLoop();
        else
            emit exitApp();
    }

    void Core::systemUpdate(SystemType type, SceneManager &manager, int64_t deltaTime)
    {
        if (_systems.find(type) == _systems.end())
            return;
        _systems[type]->update(manager, deltaTime);
    }

    void Core::onEntityAdded(std::shared_ptr<IEntity> entity)
    {
        for (auto &system : _systems)
            system.second->onEntityAdded(entity);
    }

    void Core::onEntityRemoved(std::shared_ptr<IEntity> entity)
    {
        for (auto &system : _systems)
            system.second->onEntityRemoved(entity);
    }

    void Core::setEventNetwork()
    {
        if (_systems.find(SystemType::EVENT) == _systems.end() || _systems.find(SystemType::NETWORK) == _systems.end() || _systems.find(SystemType::GAME) == _systems.end())
            throw std::runtime_error("Missing system");
        else if (_running)
            throw std::runtime_error("Can't set event network while running");

        auto netSys = dynamic_cast<NetworkClientSystem *>(_systems[SystemType::NETWORK]);
        auto evtSys = dynamic_cast<EventSystem *>(_systems[SystemType::EVENT]);
        auto gameSys = dynamic_cast<GameSystem *>(_systems[SystemType::GAME]);

        connect(evtSys, &EventSystem::writeMsg, netSys, &NetworkClientSystem::writeMsg);
        evtSys->setNetworkedEvents();
        connect(gameSys, &GameSystem::writeMsg, netSys, &NetworkClientSystem::writeMsg);
        gameSys->activateNetwork();
    }

    void Core::onClientConnection()
    {
        _sceneManager.setCurrentScene(SceneManager::SceneType::GAME);
    }
}

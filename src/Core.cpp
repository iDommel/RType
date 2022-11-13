/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Core.cpp
*/

#include "raylib.h"

#include <chrono>
#include <thread>
#include <regex>

#include "Core.hpp"
#include "systems/AudioSystem.hpp"
#include "systems/EventSystem.hpp"
#include "systems/GameSystem.hpp"
#include "systems/GraphicSystem.hpp"
#include "systems/CollideSystem.hpp"
#include "systems/ParticlesSystem.hpp"
#include "systems/NetworkClientSystem.hpp"
#include "systems/NetworkServerSystem.hpp"
#include "ArgumentError.hpp"

namespace ecs
{
    char **checkArguments(int ac, char **av)
    {
        std::regex ipRegex("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");
        std::regex portRegex("^[0-9]{1,5}$");
        if (ac == 1) {
            return nullptr;
        }
        if (ac == 3) {
            try {
                if (std::regex_match(av[1], ipRegex) && std::regex_match(av[2], portRegex)) {
                    std::stoi(av[2]);
                    return av;
                }
            } catch (std::invalid_argument &e) {
                std::cerr << "Invalid port" << std::endl;
                return nullptr;
            } catch (std::exception &e) {
                return nullptr;
            }
        }
        std::cerr << "Invalid arguments" << std::endl;
        return nullptr;
    }

    Core::Core(int ac, char **av, std::vector<SystemType> activeSystems, NetworkRole role) : QCoreApplication(ac, av)
    {
        networkRole = role;
        _ip = av[1];
        _port = std::stoi(av[2]);
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
                    auto client = new NetworkClientSystem(_ip, _port);
                    _systems[system] = client;
                    connect(this, &QCoreApplication::aboutToQuit, client, &NetworkClientSystem::destroy);
                } else if (role == NetworkRole::SERVER) {
                    auto server = new NetworkServerSystem(_ip, _port, _sceneManager);
                    _systems[system] = server;
                    connect(server, &NetworkServerSystem::changeScene, this, &Core::onChangeScene);
                }
                break;
            default:
                break;
            }
        }

        if (networkRole == NetworkRole::SERVER) {
            auto netSys = dynamic_cast<NetworkServerSystem *>(_systems[SystemType::NETWORK]);
            auto game = dynamic_cast<GameSystem *>(_systems[SystemType::GAME]);

            if (netSys == nullptr || game == nullptr)
                return;
            connect(netSys, &NetworkServerSystem::createPlayer, game, &GameSystem::createPlayer);
            connect(game, &GameSystem::writeMsg, netSys, &NetworkServerSystem::writeMsg);
        } else if (networkRole == NetworkRole::CLIENT) {
            auto netSys = dynamic_cast<NetworkClientSystem *>(_systems[SystemType::NETWORK]);
            auto game = dynamic_cast<GameSystem *>(_systems[SystemType::GAME]);

            if (netSys == nullptr || game == nullptr)
                return;
            connect(netSys, &NetworkClientSystem::createPlayer, game, &GameSystem::createPlayer);
            // connect(netSys, &NetworkClientSystem::createMissile, game, &GameSystem::createSimpleMissile);
        }
    }

    Core::~Core() {}

    void Core::run()
    {
        _running = true;
        _clock = std::chrono::high_resolution_clock::now();

        for (auto &system : _systems)
            system.second->init(_sceneManager);
        _sceneManager.setAddEntityCallback(std::bind(&Core::onEntityAdded, this, std::placeholders::_1, std::placeholders::_2));
        _sceneManager.setRemoveEntityCallback(std::bind(&Core::onEntityRemoved, this, std::placeholders::_1, std::placeholders::_2));

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

    void Core::onEntityAdded(std::shared_ptr<IEntity> entity, IScene &scene)
    {
        for (auto &system : _systems)
            system.second->onEntityAdded(entity, scene);
    }

    void Core::onEntityRemoved(std::shared_ptr<IEntity> entity, IScene &scene)
    {
        for (auto &system : _systems)
            system.second->onEntityRemoved(entity, scene);
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

    void Core::onChangeScene(SceneType scene)
    {
        _sceneManager.setCurrentScene(scene);
    }
}

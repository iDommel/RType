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

namespace rtype
{

    Core::Core(std::vector<SystemType> activeSystems)
    {
        for (auto &system : activeSystems) {
            switch (system) {
            case SystemType::GAME:
                _systems[system] = std::make_unique<GameSystem>();
                break;
            case SystemType::EVENT:
                _systems[system] = std::make_unique<EventSystem>();
                break;
            case SystemType::AUDIO:
                _systems[system] = std::make_unique<AudioSystem>();
                break;
            case SystemType::GRAPHIC:
                _systems[system] = std::make_unique<GraphicSystem>();
                break;
            case SystemType::PARTICLE:
                _systems[system] = std::make_unique<ParticlesSystem>();
                break;
            default:
                break;
            }
        }
        // _systems[SystemType::AUDIO] = std::make_unique<AudioSystem>();
        // _systems[SystemType::GAME] = std::make_unique<GameSystem>();
        // _systems[SystemType::EVENT] = std::make_unique<EventSystem>();
        // _systems[SystemType::PARTICLE] = std::make_unique<ParticlesSystem>();
        // _systems[SystemType::GRAPHIC] = std::make_unique<GraphicSystem>();
    }

    void Core::mainLoop()
    {
        auto clock = std::chrono::high_resolution_clock::now();

        for (auto &system : _systems)
            system.second->init(_sceneManager);
        _sceneManager.setAddEntityCallback(std::bind(&Core::onEntityAdded, this, std::placeholders::_1));
        _sceneManager.setRemoveEntityCallback(std::bind(&Core::onEntityRemoved, this, std::placeholders::_1));
        while (!_sceneManager.getShouldClose()) {
            auto time = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(time - clock).count();
            if (deltaTime < UPDATE_DELTA) {
                std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_DELTA - deltaTime));
                continue;
            }
            systemUpdate(SystemType::EVENT, _sceneManager, deltaTime);
            systemUpdate(SystemType::GAME, _sceneManager, deltaTime);
            systemUpdate(SystemType::AUDIO, _sceneManager, deltaTime);
            systemUpdate(SystemType::PARTICLE, _sceneManager, deltaTime);
            systemUpdate(SystemType::GRAPHIC, _sceneManager, deltaTime);
            clock = time;
        }
        for (auto &system : _systems)
            system.second->destroy();
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
}

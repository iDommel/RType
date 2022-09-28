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

namespace indie
{

    Core::Core()
    {
        _systems[SystemType::AUDIO] = std::make_unique<AudioSystem>();
        _systems[SystemType::GAME] = std::make_unique<GameSystem>();
        _systems[SystemType::EVENT] = std::make_unique<EventSystem>();
        _systems[SystemType::PARTICLE] = std::make_unique<ParticlesSystem>();
        _systems[SystemType::GRAPHIC] = std::make_unique<GraphicSystem>();
    }

    void Core::mainLoop()
    {
        auto clock = std::chrono::high_resolution_clock::now();

        for (auto &system : _systems)
            system.second->init(_sceneManager);
        _sceneManager.setAddEntityCallback(std::bind(&Core::loadEntity, this, std::placeholders::_1));
        _sceneManager.setRemoveEntityCallback(std::bind(&Core::unloadEntity, this, std::placeholders::_1));
        while (!_sceneManager.getShouldClose()) {
            auto time = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(time - clock).count();
            if (deltaTime < UPDATE_DELTA) {
                std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_DELTA - deltaTime));
                continue;
            }
            _systems[SystemType::EVENT]->update(_sceneManager, deltaTime);
            _systems[SystemType::GAME]->update(_sceneManager, deltaTime);
            _systems[SystemType::AUDIO]->update(_sceneManager, deltaTime);
            _systems[SystemType::PARTICLE]->update(_sceneManager, deltaTime);
            _systems[SystemType::GRAPHIC]->update(_sceneManager, deltaTime);
            clock = time;
        }
        for (auto &system : _systems)
            system.second->destroy();
    }

    void Core::loadEntity(std::shared_ptr<IEntity> entity)
    {
        for (auto &system : _systems)
            system.second->loadEntity(entity);
    }

    void Core::unloadEntity(std::shared_ptr<IEntity> entity)
    {
        for (auto &system : _systems)
            system.second->unloadEntity(entity);
    }
}

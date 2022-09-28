/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** ParticlesSystem
*/

#include "raylib.h"

#include <iostream>

#include "ParticlesSystem.hpp"
#include "IEntity.hpp"
#include "SceneManager.hpp"

namespace indie {
ParticlesSystem::ParticlesSystem()
{
}

void ParticlesSystem::init(SceneManager &)
{
    std::cerr << "ParticlesSystem::init" << std::endl;
}

void ParticlesSystem::update(SceneManager &manager, uint64_t deltaTime)
{
    for (auto &particlesCloud : manager.getCurrentScene()[IEntity::Tags::AESTHETIC]) {
        if (particlesCloud == nullptr || !particlesCloud->hasComponent(Component::Type::PARTICLES)) {
            manager.getCurrentScene().removeEntity(particlesCloud);
            continue;
        }
        auto particleCloud = Component::castComponent<ParticleCloud>((*particlesCloud)[IComponent::Type::PARTICLES]);
        if (!particleCloud)
            continue;
        particleCloud->addTime(deltaTime);
        if (!particleCloud->isAlive())
            manager.getCurrentScene().removeEntity(particlesCloud);
    }
}

void ParticlesSystem::destroy()
{

}

void ParticlesSystem::loadEntity(std::shared_ptr<IEntity>)
{
   
}

void ParticlesSystem::unloadEntity(std::shared_ptr<IEntity>)
{
   
}

ParticlesSystem::~ParticlesSystem()
{
}
}

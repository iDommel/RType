/*
** EPITECH PROJECT, 2022
** RType
** File description:
** NewGameSystem
*/

#include "NewGameSystem.hpp"

namespace ecs
{
    NewGameSystem::NewGameSystem()
    {
    }

    NewGameSystem::~NewGameSystem()
    {
    }

    void NewGameSystem::update(SceneManager &, uint64_t)
    {
    }

    void NewGameSystem::init(SceneManager &sceneManager)
    {
        sceneManager.addScene(createDefaultScene(), SceneManager::SceneType::GAME);
        sceneManager.setCurrentScene(SceneManager::SceneType::GAME);
    }

    void NewGameSystem::destroy()
    {
    }

    void NewGameSystem::loadEntity(std::shared_ptr<IEntity>)
    {
    }

    void NewGameSystem::unloadEntity(std::shared_ptr<IEntity>)
    {
    }

    const std::string NewGameSystem::getBinding(int)
    {
        return "";
    }

    int NewGameSystem::getTag(std::string)
    {
        return 0;
    }

    std::unique_ptr<IScene> NewGameSystem::createDefaultScene()
    {
        return std::make_unique<Scene>(std::bind(&NewGameSystem::createDefaultScene, this));
    }

}  // namespace ecs

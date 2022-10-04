/*
** EPITECH PROJECT, 2022
** RType
** File description:
** NewGameSystem
*/

#include "NewGameSystem.hpp"
#include "Position.hpp"
#include "Sprite.hpp"
#include "Entity.hpp"

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
        auto scene = std::make_unique<Scene>(std::bind(&NewGameSystem::createDefaultScene, this));
        auto entity = std::make_shared<Entity>();
        auto pos = std::make_shared<Position>(10, 10);
        auto sprite = std::make_shared<Sprite>("assets/player/sprites/player1.png");

        entity->addComponent(pos)
                .addComponent(sprite);
        scene->addEntity(entity);
        return scene;
    }

}  // namespace ecs

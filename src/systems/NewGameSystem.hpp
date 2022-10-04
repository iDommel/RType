/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** NewGameSystem.hpp
*/

#ifndef NEW_GAME_SYSTEM_HPP
#define NEW_GAME_SYSTEM_HPP

#include <chrono>

#include "ISystem.hpp"
#include "SceneManager.hpp"
#include "Scene.hpp"

namespace ecs
{
    class IEntity;
    class Scene;
    class Position;

    class NewGameSystem : public ISystem
    {
    public:
        NewGameSystem();
        ~NewGameSystem();

        void init(SceneManager &manager) final;
        void update(SceneManager &manager, uint64_t deltaTime) final;
        void destroy() final;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         */
        void loadEntity(std::shared_ptr<IEntity> entity) final;
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void unloadEntity(std::shared_ptr<IEntity> entity) final;

        static const std::string getBinding(int keyboard);

        static int getTag(std::string key);

    private:
        std::unique_ptr<IScene> createDefaultScene();
        // CollideSystem _collideSystem;
        // AISystem _aiSystem;
    };
}

#endif /* NEW_GAME_SYSTEM_HPP */
/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** System.hpp
*/

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <memory>

namespace indie {

    class SceneManager;
    class IEntity;

    class ISystem
    {
    public:

        /**
         * @brief Initialize the system before the game loop
         * @param manager The scene manager
         */
        virtual void init(SceneManager &manager) = 0;
        /**
         * @brief Update the system called every time deltaTime >= UPDATE_DELTA
         * @param manager The scene manager
         * @param deltaTime The time since the last update
         */
        virtual void update(SceneManager &manager, uint64_t deltaTime) = 0;
        /**
         * @brief Destroy the system at the end of the game loop
         */
        virtual void destroy() = 0;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         */
        virtual void loadEntity(std::shared_ptr<IEntity> entity) = 0;
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        virtual void unloadEntity(std::shared_ptr<IEntity> entity) = 0;

    };

}

#endif /* SYSTEM_HPP */
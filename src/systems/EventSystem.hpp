/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** EventSystem.hpp
*/

#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include "ISystem.hpp"
#include "SceneManager.hpp"

namespace indie
{
    class EventListener;
    class EventSystem : public ISystem
    {
    public:
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

        static void reloadScene(SceneManager &manager, SceneManager::SceneType sceneType);

    private:
        void handleKeyboard(SceneManager &, std::shared_ptr<EventListener> listener);
        void handleMouse(SceneManager &, std::shared_ptr<EventListener> listener);
        void handleGamepad(SceneManager &, std::shared_ptr<EventListener> listener, int nb);
        void handleGamepadSticks(SceneManager &, std::shared_ptr<EventListener> listener, int nb);

        /// @brief this number is from looking into the source files from the raylib
        const int _maxGamepads = 4;
        static std::map<int, std::vector<std::shared_ptr<EventListener>>> _listeners;
    };

}

#endif /* EVENT_SYSTEM_HPP */
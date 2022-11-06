/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** EventSystem.hpp
*/

#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include "Message.hpp"
#include "ISystem.hpp"
#include "SceneManager.hpp"

#include <QtCore>  // for networked event handling

namespace ecs
{
    class EventListener;
    class EventSystem : public QObject, public ISystem
    {
        Q_OBJECT

    public:
        EventSystem();

        void init(SceneManager &manager) final;
        void update(SceneManager &manager, uint64_t deltaTime) final;
        void destroy() final;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         * @param scene Scene to add entity into
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity, SceneType scene) final;
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity) final;

        static void reloadScene(SceneManager &manager, SceneType sceneType);

        void setNetworkedEvents();

    signals:
        void writeMsg(const Message &message);

    private:
        void handleKeyboard(SceneManager &, std::shared_ptr<EventListener> listener);
        void handleMouse(SceneManager &, std::shared_ptr<EventListener> listener);
        void handleGamepad(SceneManager &, std::shared_ptr<EventListener> listener, int nb);
        void handleGamepadSticks(SceneManager &, std::shared_ptr<EventListener> listener, int nb);

        /// @brief this number is from looking into the source files from the raylib
        const int _maxGamepads = 4;
        static std::map<int, std::vector<std::shared_ptr<EventListener>>> _listeners;
        /// @brief Set at true if events should be send to a server.
        bool _netEvt = false;
    };

}

#endif /* EVENT_SYSTEM_HPP */
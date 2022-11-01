/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** ANetworkSystem.hpp
*/

#ifndef NETWORK_SYSTEM_HPP
#define NETWORK_SYSTEM_HPP

#include "ISystem.hpp"
#include "UdpSocket.hpp"
#include <QtCore>

#define PING_TIMEOUT        5000

class QHostAddress;
class QThread;

namespace ecs
{
    enum class NetworkRole {
        UNDEFINED = -1,
        SERVER,
        CLIENT
    };

    class ANetworkSystem : public QObject, public ISystem
    {
        Q_OBJECT

    public:
        enum class ClientState {
            UNDEFINED = -1,
            CONNECTED,
            READYTOPLAY,
            PLAYING
        };

        ANetworkSystem();

        virtual void init(SceneManager &manager) = 0;
        virtual void update(SceneManager &manager, uint64_t deltaTime) = 0;
        virtual void destroy() = 0;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         * @param scene Scene to add entity into
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity, SceneType scene);
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity);

    protected:
        QHostAddress _serverAddr;
        unsigned short _port;

        UdpSocket *_socket;
    };

}

#endif /* NETWORK_SYSTEM_HPP */
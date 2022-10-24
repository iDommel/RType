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
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity, SceneType scene);
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity);

    // public slots:
    //     /// @brief Sends msg through the UdpSocket
    //     /// @param msg Message to send
    //     /// @note Depending on the network role; sends to server or all clients
    //     void writeMsg(const std::string &msg);
    //     /// @brief Puts received message in the system's queue
    //     /// @param msg Message received
    //     void putMsgInQueue(std::string msg);

    // signals:
    //     void clientConnection();

    protected:

        QHostAddress _serverAddr;
        unsigned short _port;

        UdpSocket *_socket;
    };

}

#endif /* NETWORK_SYSTEM_HPP */
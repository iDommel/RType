/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkSystem.hpp
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

    class NetworkSystem : public QObject, public ISystem
    {
        Q_OBJECT

    public:

        NetworkSystem();

        void init(SceneManager &manager);
        void update(SceneManager &manager, uint64_t deltaTime);
        void destroy();

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity);
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity);

    public slots:
        /// @brief Sends msg through the UdpSocket
        /// @param msg Message to send
        /// @note Depending on the network role; sends to server or all clients
        void writeMsg(const std::string &msg);
        /// @brief Puts received message in the system's queue
        /// @param msg Message received
        void putMsgInQueue(std::string msg);

    signals:
        void clientConnection();

    protected:

        QHostAddress _serverAddr;
        unsigned short _port;

        UdpSocket *_socket;
        std::vector<std::string> _msgQueue;
    };

}

#endif /* NETWORK_SYSTEM_HPP */
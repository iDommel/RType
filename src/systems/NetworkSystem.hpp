/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkSystem.hpp
*/

#ifndef NETWORK_SYSTEM_HPP
#define NETWORK_SYSTEM_HPP

#include <map>

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

        NetworkSystem(NetworkRole role);

        void init(SceneManager &manager) final;
        void update(SceneManager &manager, uint64_t deltaTime) final;
        void destroy() final;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         */
        void onEntityAdded(std::shared_ptr<IEntity> entity) final;
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void onEntityRemoved(std::shared_ptr<IEntity> entity) final;

        /// @brief Blocks the thread for ms milliseconds until a message is received
        /// @param msg Message to be received
        /// @param ms Milliseconds to wait; 30 000 by default; -1 no timeout
        /// @return Returns true if the specified message was received, false otherwise
        bool waitForMsg(std::string msg, int ms = 30000);

    public slots:
        void writeMsg(const std::string &msg);
        void putMsgInQueue(std::string msg);

    signals:
        void clientConnection();

    private:
        QHostAddress _serverAddr;
        unsigned short _port;

        NetworkRole _role = NetworkRole::UNDEFINED;
        UdpSocket *_socket;
        std::vector<std::pair<QString /*addr*/, unsigned short /*port*/>> _senders;

        std::vector<std::string> _msgQueue;
    };

}

#endif /* NETWORK_SYSTEM_HPP */
/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkServerSystem.hpp
*/

#ifndef NETWORK_SERVER_SYSTEM_HPP
#define NETWORK_SERVER_SYSTEM_HPP

#include "NetworkSystem.hpp"
#include <vector>

namespace ecs {

    class NetworkServerSystem : public NetworkSystem
    {

        Q_OBJECT

    public:

        void init(SceneManager &manager);
        void update(SceneManager &manager, uint64_t deltaTime);
        void destroy() {};

    public slots:
        /// @brief Sends msg through the UdpSocket to all connected clients
        /// @param msg Message to send
        void writeMsg(const std::string &msg);

        /// @brief Sends msg through the UdpSocket to the specified client
        /// @param msg Message to send
        /// @param clientId Client ID to send msg to
        void writeToClient(const std::string &msg, int clientId);

        /// @brief Puts received message in the system's queue
        /// @param msg Message received
        void putMsgInQueue(std::string msg);

    signals:
        void clientConnection();

    private:

        void connectClient(QString addr, unsigned short port);
        void deconnectClient(QString addr, unsigned short port);

        /// @brief Gets a player event message and moves entities accordingly
        /// @param msg The received message
        void handlePlayerEvent(SceneManager &manager, const std::string &msg, uint64_t deltaTime);

        std::vector<std::pair<QString /*addr*/, unsigned short /*port*/>> _senders;

    };

}

#endif /* NETWORK_SERVER_SYSTEM_HPP */
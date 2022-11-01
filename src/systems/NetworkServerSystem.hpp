/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkServerSystem.hpp
*/

#ifndef NETWORK_SERVER_SYSTEM_HPP
#define NETWORK_SERVER_SYSTEM_HPP

#include "ANetworkSystem.hpp"
#include <vector>
#include <map>

namespace ecs
{

    class NetworkServerSystem : public ANetworkSystem
    {
        Q_OBJECT

    public:
        void init(SceneManager &manager);
        /// @brief Reads all received messages and processes them
        void update(SceneManager &manager, uint64_t deltaTime);
        void destroy(){};

    public slots:
        /// @brief Sends msg through the UdpSocket to all connected clients
        /// @param msg Message to send
        void writeMsg(const Message &msg);

        /// @brief Sends msg through the UdpSocket to the specified client
        /// @param msg Message to send
        /// @param clientId Client ID to send msg to
        void writeToClient(const std::string &msg, int clientId);

        /// @brief Puts received message in the system's queue
        /// @param msg Message received
        void putMsgInQueue(std::string msg);

    signals:
        void changeScene(SceneManager::SceneType scene);

    private:
        void connectClient(QString addr, unsigned short port);
        void deconnectClient(QString addr, unsigned short port);
        void setClientReady(std::pair<QString /*addr*/, unsigned short /*port*/> client);

        /// @brief Gets a player event message and moves entities accordingly
        /// @param msg The received message
        void handlePlayerEvent(SceneManager &manager, const std::string &msg, uint64_t deltaTime);
        void handlePlayerEvent(SceneManager &manager, const Message &msg, uint64_t deltaTime);

        std::map<std::string, std::pair<QString /*addr*/, unsigned short /*port*/>> _msgQueue;
        std::vector<std::pair<QString /*addr*/, unsigned short /*port*/>> _senders;
        std::map<std::pair<QString /*addr*/, unsigned short /*port*/>, ANetworkSystem::ClientState> _states;
    };

}

#endif /* NETWORK_SERVER_SYSTEM_HPP */
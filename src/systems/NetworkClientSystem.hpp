/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkClientSystem.hpp
*/

#ifndef _NETWORK_CLIENT_SYSTEM_HPP
#define _NETWORK_CLIENT_SYSTEM_HPP

#include "ANetworkSystem.hpp"

namespace ecs {

    class NetworkClientSystem : public ANetworkSystem
    {

        Q_OBJECT

    public:

        void init(SceneManager &manager);
        void update(SceneManager &manager, uint64_t deltaTime);
        void destroy();

    public slots:
        /// @brief Sends msg to server
        /// @param msg Message to send
        void writeMsg(const std::string &msg);

        /// @brief Puts received message in the system's queue
        /// @param msg Message received
        void putMsgInQueue(std::string msg);

    private:
        /// @brief Gets a player event message and moves entities accordingly
        /// @param msg The received message
        void handlePlayerEvent(SceneManager &manager, const std::string &msg, uint64_t deltaTime);


        std::vector<std::string> _msgQueue;
        bool _connected = false;
    };

}

#endif /* _NETWORK_CLIENT_SYSTEM_HPP */
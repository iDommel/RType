/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkClientSystem.hpp
*/

#ifndef _NETWORK_CLIENT_SYSTEM_HPP
#define _NETWORK_CLIENT_SYSTEM_HPP

#include "ANetworkSystem.hpp"
#include "IScene.hpp"
#include "Position.hpp"

namespace ecs
{

    class NetworkClientSystem : public ANetworkSystem
    {
        Q_OBJECT

    public:
        /// @brief NetworkClientSystem constructor
        /// @param serverAddr Server IP address
        /// @param port Server port number
        NetworkClientSystem(std::string serverAddr, unsigned short port);

        void init(SceneManager &manager);
        /** @brief Reads all received messages and processes them
         *  @note If not connected writes to server to connect.
         *  @see NetworkServerSystem::update()
         */
        void update(SceneManager &manager, uint64_t deltaTime);
        void destroy();

    signals:
        void createPlayer(IScene &scene, int keyRight, int keyLeft, int keyUp, int keyDown, int keyBomb, long unsigned int id, Position pos, bool isMe);
        void createMissile(IScene &scene, long unsigned int id, Position pos);

    public slots:
        /// @brief Sends msg to server
        /// @param msg Message to send
        void writeMsg(const Message &msg);

        /// @brief Puts received message in the system's queue
        /// @param msg Message received
        void putMsgInQueue(Message msg);

    private slots:
        /// @brief Notifies the server the client is alive when PING_TIMEOUT ms have passed without sending a message
        void onPingTimeout();

    private:
        /// @brief Gets a player event message and moves entities accordingly
        /// @param msg The received message
        void handlePlayerEvent(SceneManager &manager, std::string msg, uint64_t deltaTime);

        /// @brief Gets a player event message and moves entities accordingly
        /// @param msg The received message
        void handlePlayerEvent(SceneManager &manager, const Message &msg, uint64_t deltaTime);
        void handleMissileUpdate(SceneManager &sceneManager, const Message &msg, uint64_t dt);
        void handleEnemyUpdate(SceneManager &sceneManager, const Message &msg, uint64_t dt);

        void removePlayer(long unsigned int id, SceneManager &manager);
        void processEntityMessage(Message &msg, SceneManager &sceneManager, uint64_t deltaTime);

        std::vector<Message> _msgQueue;
        bool _connected = false;
        QTimer _timer;
    };

}

#endif /* _NETWORK_CLIENT_SYSTEM_HPP */
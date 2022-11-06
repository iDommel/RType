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
#include <unordered_map>
#include "Position.hpp"

namespace ecs
{

    class NetworkServerSystem : public ANetworkSystem
    {
        Q_OBJECT

    public:
        /// @brief NetworkServerSystem constructor
        /// @param addr Server IP address
        /// @param port Server port number
        NetworkServerSystem(std::string addr, unsigned short port, SceneManager &sceneManager);

        void init(SceneManager &manager);
        /// @brief Reads all received messages and processes them
        void update(SceneManager &manager, uint64_t deltaTime);
        void destroy(){};

        void deconnectClientTimedout(std::pair<QString, unsigned short> client);

    public slots:
        /// @brief Sends msg through the UdpSocket to all connected clients
        /// @param msg Message to send
        void writeMsg(const Message &msg);

        /// @brief Sends msg through the UdpSocket to the specified client
        /// @param msg Message to send
        /// @param clientId Client ID to send msg to
        void writeToClient(Message msg, int clientId);
        void writeToClient(Message msg, std::pair<QString /*addr*/, unsigned short /*port*/> client);

        /// @brief Puts received message in the system's queue
        /// @param msg Message received
        void putMsgInQueue(Message msg);

    signals:
        void changeScene(SceneType scene);
        void createPlayer(IScene &scene, int keyRight, int keyLeft, int keyUp, int keyDown, int keyBomb, long unsigned int id, Position pos, bool isMe);

    private:
        void connectClient(std::pair<QString, unsigned short> client);
        void deconnectClient(std::pair<QString, unsigned short> client);
        void setClientReady(std::pair<QString /*addr*/, unsigned short /*port*/> client, SceneManager &sceneManager);
        void removePlayer(int id);

        /// @brief Gets a player event message and moves entities accordingly
        /// @param manager SceneManager reference
        /// @param msg The received message
        /// @param id Player ID
        /// @param deltaTime Time in milliseconds since last update
        void handlePlayerEvent(SceneManager &manager, const Message &msg, int id, uint64_t deltaTime);

        void sendServerUpdates(SceneManager &manager, uint64_t deltaTime);
        std::vector<Message> _msgQueue;
        std::vector<std::pair<QString /*addr*/, unsigned short /*port*/>> _senders;
        std::map<std::pair<QString /*addr*/, unsigned short /*port*/>, ANetworkSystem::ClientState> _states;
        int _players = 0;
        std::map<std::pair<QString /*addr*/, unsigned short /*port*/>, unsigned long int> _playersId;
        std::map<std::pair<QString, unsigned short> /* client */, QTimer> _timers;

        SceneManager &_sceneManager;
    };

}

#endif /* NETWORK_SERVER_SYSTEM_HPP */
/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkServerSystem.cpp
*/

#include "SceneManager.hpp"
#include "NetworkServerSystem.hpp"
#include "Core.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "GameSystem.hpp"

namespace ecs
{

    NetworkServerSystem::NetworkServerSystem(std::string addr, unsigned short port, SceneManager &sceneManager) : _sceneManager(sceneManager)
    {
        _serverAddr = QHostAddress(QString(addr.c_str()));
        _port = port;
    }

    void NetworkServerSystem::init(SceneManager &)
    {
        std::cerr << "NetworkServerSystem::init" << std::endl;
        _socket = new UdpSocket(this, _serverAddr, _port);
        connect(_socket, &UdpSocket::transferMsgToSystem, this, &NetworkServerSystem::putMsgInQueue);
    }

    void NetworkServerSystem::update(SceneManager &manager, uint64_t dt)
    {
        for (auto &s : _msgQueue) {
            if (s.getMessageType() == MessageType::TEXTMESSAGE)
                std::cout << s.getText() << std::endl;
            if (s.getMessageType() == MessageType::TEXTMESSAGE && s.getText() == DISCONNECTED)
                deconnectClient(s.getSender());
            else if (s.getMessageType() == MessageType::TEXTMESSAGE && s.getText() == WAIT_CONNECTION && manager.getCurrentSceneType() == SceneType::LOBBY)
                connectClient(s.getSender());
            else if (s.getMessageType() == MessageType::TEXTMESSAGE && s.getText() == READY)
                setClientReady(s.getSender(), manager);
            else if (s.getEventType() == EventType::KEYBOARD) {
                handlePlayerEvent(manager, s, _playersId[s.getSender()], dt);
            }
        }
        _msgQueue.clear();
    }

    void NetworkServerSystem::handlePlayerEvent(SceneManager &manager, const Message &message, int id, uint64_t dt)
    {
        auto players = manager.getCurrentScene()[IEntity::Tags::PLAYER];
        EventType msgType = message.getEventType();
        KeyState keyState = message.getKeyState();
        KeyboardKey key = message.getKeyboardKey();
        std::shared_ptr<IEntity> entity = nullptr;
        std::shared_ptr<Player> playerComp = nullptr;

        for (auto &player : players) {
            playerComp = Component::castComponent<Player>((*player)[IComponent::Type::PLAYER]);
            if (playerComp->getId() == id) {
                entity = player;
                break;
            }
        }

        auto pos = Component::castComponent<Position>((*entity)[IComponent::Type::POSITION]);

        switch (key) {
        case KEY_RIGHT:
            if (keyState == KeyState::DOWN)
                playerComp->moveRight(manager, entity, dt);
            else
                playerComp->stopRight(manager, entity, dt);
            break;
        case KEY_LEFT:
            if (keyState == KeyState::DOWN)
                playerComp->moveLeft(manager, entity, dt);
            else
                playerComp->stopLeft(manager, entity, dt);
            break;
        case KEY_UP:
            if (keyState == KeyState::DOWN)
                playerComp->moveUp(manager, entity, dt);
            else
                playerComp->stopUp(manager, entity, dt);
            break;
        case KEY_DOWN:
            if (keyState == KeyState::DOWN)
                playerComp->moveDown(manager, entity, dt);
            else
                playerComp->stopDown(manager, entity, dt);
            break;
        }
        writeMsg(Message("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y)));
    }
/*
    void NetworkServerSystem::handlePlayerEvent(SceneManager &manager, std::string msg, uint64_t dt)
    {
        auto players = manager.getCurrentScene()[IEntity::Tags::PLAYER];

        msg.erase(0, 7);
        std::cerr << msg << std::endl;
        int playerId = std::stoi(msg.substr(0, msg.find(' ')));
        msg.erase(0, msg.find(' ') + 1);
        std::cerr << msg << std::endl;
        std::string axis = msg.substr(0, msg.find(' '));
        msg.erase(0, msg.find(' ') + 1);
        std::cerr << msg << std::endl;
        std::string action = msg;

        if (axis == "RIGHT" && action == "DOWN") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (playerComp->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveRight(manager, p, dt);
                // std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }
        if (axis == "RIGHT" && action == "RELEASED") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (playerComp->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->stopRight(manager, p, dt);
                // std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }
        if (axis == "LEFT" && action == "DOWN") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (playerComp->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveLeft(manager, p, dt);
                // std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }

        if (axis == "LEFT" && action == "RELEASED") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (playerComp->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->stopLeft(manager, p, dt);
                // std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }

        if (axis == "UP" && action == "DOWN") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (playerComp->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveUp(manager, p, dt);
                // std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }

        if (axis == "UP" && action == "RELEASED") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (playerComp->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->stopUp(manager, p, dt);
                // std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }

        if (axis == "DOWN" && action == "DOWN") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (playerComp->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveDown(manager, p, dt);
                // std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }

        if (axis == "DOWN" && action == "RELEASED") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (playerComp->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->stopDown(manager, p, dt);
                // std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }
    }
*/
    void NetworkServerSystem::writeMsg(const Message &msg)
    {
        for (auto &client : _senders)
            _socket->write(msg, QHostAddress(client.first), client.second);
    }

    void NetworkServerSystem::writeToClient(const std::string &msg, int clientId)
    {
        _socket->write(msg, QHostAddress(_senders[clientId].first), _senders[clientId].second);
    }

    void NetworkServerSystem::writeToClient(const std::string &msg, std::pair<QString /*addr*/, unsigned short /*port*/> client)
    {
        _socket->write(msg, QHostAddress(client.first), client.second);
    }

    void NetworkServerSystem::putMsgInQueue(Message msg)
    {
        QString addr = _socket->getLastAddress().toString();
        unsigned short port = _socket->getLastPort();
        std::pair<QString, unsigned short> client = std::make_pair(addr, port);

        if (_timers.find(client) != _timers.end())
            _timers[client].start(PING_TIMEOUT * 2);

        msg.setSender(client);
        _msgQueue.push_back(msg);
    }

    void NetworkServerSystem::connectClient(std::pair<QString, unsigned short> client)
    {
        std::cerr << "coucou"<< std::endl;
        if (_senders.size() >= NB_CLIENTS_MAX)
            return;
        for (auto &s : _senders) {
            if (s == client)
                return;
        }
        _senders.push_back(client);
        _states[client] = ClientState::CONNECTED;
        _timers[client];
        connect(&_timers[client], &QTimer::timeout, std::bind(&NetworkServerSystem::deconnectClientTimedout, this, client));
        _timers[client].start(PING_TIMEOUT * 2);
        _socket->write(CONNECTION_OK, QHostAddress(client.first), client.second);
    }

    void NetworkServerSystem::deconnectClient(std::pair<QString, unsigned short> client)
    {
        unsigned i = 0;

        for (auto s : _senders) {
            if (s == client) {
                _senders.erase(_senders.begin() + i);
                _states.erase(s);
                _timers.erase(s);
                if (_sceneManager.getCurrentSceneType() == SceneType::GAME)
                    removePlayer(_playersId[s]);
                std::cerr << "Removed client" << std::endl;
                break;
            }
            i++;
        }
    }

    void NetworkServerSystem::deconnectClientTimedout(std::pair<QString, unsigned short> client)
    {
        deconnectClient(client);
    }

    void NetworkServerSystem::removePlayer(int id)
    {
        for (auto entity : _sceneManager.getScene(SceneType::GAME)[IEntity::Tags::PLAYER]) {
            auto player = Component::castComponent<Player>((*entity)[IComponent::Type::PLAYER]);
            if (player->getId() == id) {
                _sceneManager.getCurrentScene().removeEntity(entity);
                break;
            }
        }
        for (auto &s : _senders)
            writeMsg(Message(std::string(RM_PLAYER) + " " + std::to_string(id)));
    }

    void NetworkServerSystem::setClientReady(std::pair<QString /*addr*/, unsigned short /*port*/> client, SceneManager &manager)
    {
        if (_states[client] != ClientState::CONNECTED)
            return;
        _states[client] = ClientState::READYTOPLAY;
        // add new player entity
        _playersId[client] = ++_players;

        // check if all players are ready
        for (auto s : _states) {
            if (s.second != ClientState::READYTOPLAY)
                return;
        }

        // Create players inside clients
        for (auto &client : _senders) {
            unsigned int id = _playersId[client];
            emit createPlayer(manager.getScene(SceneType::GAME), KEY_Q, KEY_D, KEY_Z, KEY_S, KEY_RIGHT_CONTROL, id, GameSystem::_playerSpawns[id], false);
            for (auto &player : _senders) {
                if (player == client)
                    writeToClient(std::string(CR_ME) + std::to_string(id), player);
                else
                    writeToClient(std::string(CR_PLAYER) + std::to_string(id), player);
            }
        }

        // notify clients game can start
        writeMsg(Message(READY));
        emit changeScene(SceneType::GAME);
    }

}
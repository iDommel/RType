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

    void NetworkServerSystem::init(SceneManager &)
    {
        std::cerr << "NetworkServerSystem::init" << std::endl;
        _socket = new UdpSocket(this, _serverAddr, _port);
        connect(_socket, &UdpSocket::transferMsgToSystem, this, &NetworkServerSystem::putMsgInQueue);
    }

    void NetworkServerSystem::update(SceneManager &manager, uint64_t dt)
    {
        for (auto &s : _msgQueue) {
            std::cerr << s.first << std::endl;
            if (s.first == DISCONNECTED)
                deconnectClient(s.second.first, s.second.second);
            else if (s.first == WAIT_CONNECTION && manager.getCurrentSceneType() == SceneType::LOBBY)
                connectClient(s.second.first, s.second.second);
            else if (s.first == READY)
                setClientReady(s.second, manager);
            if (s.first.rfind("PLAYER ", 0) == 0) {
                handlePlayerEvent(manager, s.first, dt);
            }
        }
        _msgQueue.clear();
    }

    void NetworkServerSystem::handlePlayerEvent(SceneManager &manager, const Message &message, uint64_t dt)
    {
        auto players = manager.getCurrentScene()[IEntity::Tags::PLAYER];
        EventType msgType = message.getEventType();
        KeyState keyState = message.getKeyState();
        KeyboardKey key = message.getKeyboardKey();

        if (key == KEY_RIGHT && keyState == KeyState::DOWN) {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveRight(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }
    }

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
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
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
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
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
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
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
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
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
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
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
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
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
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
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
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER "+ std::to_string(playerId) +" POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
                break;
            }
        }
    }

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

    void NetworkServerSystem::putMsgInQueue(std::string msg)
    {
        QString addr = _socket->getLastAddress().toString();
        unsigned short port = _socket->getLastPort();

        if (_timers.find(std::make_pair(addr, port)) != _timers.end())
            _timers[std::make_pair(addr, port)].start(PING_TIMEOUT * PING_TIMEOUT);
        if (!msg.empty())
            _msgQueue[msg] = std::make_pair(addr, port);
    }

    void NetworkServerSystem::connectClient(QString addr, unsigned short port)
    {
        if (_senders.size() >= NB_CLIENTS_MAX)
            return;
        for (auto &s : _senders) {
            if (s.first == addr && s.second == port)
                return;
        }
        auto client = std::make_pair(addr, port);
        _senders.push_back(client);
        _states[client] = ClientState::CONNECTED;
        _timers[client];
        connect(&_timers[client], &QTimer::timeout, std::bind(&NetworkServerSystem::deconnectClientTimedout, this, client));
        _timers[client].start(PING_TIMEOUT * PING_TIMEOUT);
        _socket->write(CONNECTION_OK, QHostAddress(addr), port);
    }

    void NetworkServerSystem::deconnectClient(QString addr, unsigned short port)
    {
        unsigned i = 0;
        for (auto s : _senders) {
            if (s.first == addr && s.second == port) {
                _senders.erase(_senders.begin() + i);
                _states.erase(s);
                _timers.erase(s);
                std::cerr << "Removed client" << std::endl;
                break;
            }
            i++;
        }
    }

    void NetworkServerSystem::deconnectClientTimedout(std::pair<QString, unsigned short> client)
    {
        deconnectClient(client.first, client.second);
    }

    void NetworkServerSystem::setClientReady(std::pair<QString /*addr*/, unsigned short /*port*/> client, SceneManager &manager)
    {
        if (_states[client] != ClientState::CONNECTED)
            return;
        _states[client] = ClientState::READYTOPLAY;
        // add new player entity
        _playersId[client] = ++_players;
        emit createPlayer(manager.getScene(SceneType::GAME), KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL, _players, GameSystem::_playerSpawns[_players], false);
        // writeMsg(std::string(CR_PLAYER) + std::to_string(_players));

        for (auto &s : _senders) {
            if (s == client)
                writeToClient(std::string(CR_ME) + std::to_string(_players), s);
            else
                writeToClient(std::string(CR_PLAYER) + std::to_string(_players), s);
        }

        // check if all players are ready
        for (auto s : _states) {
            if (s.second != ClientState::READYTOPLAY)
                return;
        }
        // notify clients game can start
        writeMsg(READY);
        emit changeScene(SceneType::GAME);
    }

}
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
            if (s.first == DECONNECTED)
                deconnectClient(s.second.first, s.second.second);
            else if (s.first == WAIT_CONNECTION)
                connectClient(s.second.first, s.second.second);
            else if (s.first == READY)
                setClientReady(s.second);
            if (s.first.rfind("PLAYER ", 0) == 0) {
                handlePlayerEvent(manager, s.first, dt);
            }
        }
        _msgQueue.clear();
    }

    void NetworkServerSystem::handlePlayerEvent(SceneManager &manager, const std::string &msg, uint64_t dt)
    {
        auto players = manager.getCurrentScene()[IEntity::Tags::PLAYER];
        std::string axis = msg.substr(7, msg.find(' ', 7) - 7);
        std::string action = msg.substr(msg.find(' ', 7) + 1, msg.find(' ', msg.find(' ', 7) + 1) - msg.find(' ', 7) - 1);
        if (axis == "RIGHT" && action == "DOWN") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveRight(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }
        if (axis == "RIGHT" && action == "RELEASED") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->stopRight(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }
        if (axis == "LEFT" && action == "DOWN") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveLeft(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }

        if (axis == "LEFT" && action == "RELEASED") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->stopLeft(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }

        if (axis == "UP" && action == "DOWN") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveUp(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }

        if (axis == "UP" && action == "RELEASED") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->stopUp(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }

        if (axis == "DOWN" && action == "DOWN") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->moveDown(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }

        if (axis == "DOWN" && action == "RELEASED") {
            for (auto &p : players) {
                auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                playerComp->stopDown(manager, p, dt);
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
            }
        }
    }

    void NetworkServerSystem::writeMsg(const std::string &msg)
    {
        for (auto &client : _senders)
            _socket->write(msg, QHostAddress(client.first), client.second);
    }

    void NetworkServerSystem::writeToClient(const std::string &msg, int clientId)
    {
        _socket->write(msg, QHostAddress(_senders[clientId].first), _senders[clientId].second);
    }

    void NetworkServerSystem::putMsgInQueue(std::string msg)
    {
        QString addr = _socket->getLastAddress().toString();
        unsigned short port = _socket->getLastPort();

        if (!msg.empty())
            _msgQueue[msg] = std::make_pair(addr, port);
    }

    void NetworkServerSystem::connectClient(QString addr, unsigned short port)
    {
        for (auto &s : _senders) {
            if (s.first == addr && s.second == port)
                return;
        }
        _senders.push_back(std::make_pair(addr, port));
        _states[std::make_pair(addr, port)] = ClientState::CONNECTED;
        _socket->write(CONNECTION_OK, QHostAddress(addr), port);
        // TODO: This isn't good
        emit clientConnection();
    }

    void NetworkServerSystem::deconnectClient(QString addr, unsigned short port)
    {
        unsigned i = 0;
        for (auto s : _senders) {
            if (s.first == addr && s.second == port) {
                _senders.erase(_senders.begin() + i);
                _states.erase(s);
                std::cerr << "Removed client" << std::endl;
                break;
            }
            i++;
        }
    }

    void NetworkServerSystem::setClientReady(std::pair<QString /*addr*/, unsigned short /*port*/> client)
    {
        _states[client] = ClientState::READYTOPLAY;
        for (auto s : _states) {
            if (s.second != ClientState::READYTOPLAY)
                return;
        }
        writeMsg(READY);
    }

}
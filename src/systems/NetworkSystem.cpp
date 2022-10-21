/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkSystem.cpp
*/

#include "NetworkSystem.hpp"
#include <QtNetwork>
#include <iostream>
#include <thread>
#include "SceneManager.hpp"
#include "Core.hpp"
#include "Player.hpp"
#include "Position.hpp"

namespace ecs
{

    NetworkRole Core::networkRole;

    NetworkSystem::NetworkSystem() : QObject(nullptr)
    {
        _serverAddr = QHostAddress::LocalHost;
        _port = 8080;
        if (Core::networkRole == NetworkRole::UNDEFINED)
            throw std::invalid_argument("NetworkRole undefined");
    }

    // void NetworkSystem::init(SceneManager &)
    // {
    //     std::cerr << "NetworkSystem::init" << std::endl;
    //     if (Core::networkRole == NetworkRole::CLIENT) {
    //         _socket = new UdpSocket(this, QHostAddress::AnyIPv4, 0);
    //     } else if (Core::networkRole == NetworkRole::SERVER) {
    //         _socket = new UdpSocket(this, _serverAddr, _port);
    //     }
    //     connect(_socket, &UdpSocket::transferMsgToSystem, this, &NetworkSystem::putMsgInQueue);
    // }

    // void NetworkSystem::update(SceneManager &manager, uint64_t dt)
    // {
    //     static bool waitCo = false;

    //     if (manager.getCurrentSceneType() == SceneManager::SceneType::CONNECTION && Core::networkRole == NetworkRole::CLIENT && !_connected && !waitCo) {
    //         writeMsg(WAIT_CONNECTION);
    //         waitCo = true;
    //     }

    //     for (auto &s : _msgQueue) {
    //         std::cerr << s << std::endl;
    //         if (waitCo && !_connected && s == CONNECTION_OK) {
    //             emit clientConnection();
    //         }
    //         if (s.rfind("PLAYER ", 0) == 0) {
    //             handlePlayerEvent(manager, s, dt);
    //         }
    //     }
    //     _msgQueue.clear();
    // }

    // void NetworkSystem::handlePlayerEvent(SceneManager &manager, const std::string &msg, uint64_t dt)
    // {
    //     auto players = manager.getCurrentScene()[IEntity::Tags::PLAYER];
    //     std::string axis = msg.substr(7, msg.find(' ', 7) - 7);
    //     std::string action = msg.substr(msg.find(' ', 7) + 1, msg.find(' ', msg.find(' ', 7) + 1) - msg.find(' ', 7) - 1);
    //     if (axis == "RIGHT" && action == "DOWN") {
    //         for (auto &p : players) {
    //             auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             playerComp->moveRight(manager, p, dt);
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //             writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
    //         }
    //     }
    //     if (axis == "RIGHT" && action == "RELEASED") {
    //         for (auto &p : players) {
    //             auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             playerComp->stopRight(manager, p, dt);
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //             writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
    //         }
    //     }
    //     if (axis == "LEFT" && action == "DOWN") {
    //         for (auto &p : players) {
    //             auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             playerComp->moveLeft(manager, p, dt);
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //             writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
    //         }
    //     }

    //     if (axis == "LEFT" && action == "RELEASED") {
    //         for (auto &p : players) {
    //             auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             playerComp->stopLeft(manager, p, dt);
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //             writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
    //         }
    //     }

    //     if (axis == "UP" && action == "DOWN") {
    //         for (auto &p : players) {
    //             auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             playerComp->moveUp(manager, p, dt);
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //             writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
    //         }
    //     }

    //     if (axis == "UP" && action == "RELEASED") {
    //         for (auto &p : players) {
    //             auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             playerComp->stopUp(manager, p, dt);
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //             writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
    //         }
    //     }

    //     if (axis == "DOWN" && action == "DOWN") {
    //         for (auto &p : players) {
    //             auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             playerComp->moveDown(manager, p, dt);
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //             writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
    //         }
    //     }

    //     if (axis == "DOWN" && action == "RELEASED") {
    //         for (auto &p : players) {
    //             auto playerComp = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             playerComp->stopDown(manager, p, dt);
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //             writeMsg("PLAYER POS " + std::to_string(pos->x) + " " + std::to_string(pos->y));
    //         }
    //     }

    //     if (axis == "POS") {
    //         std::cerr << "POS: " << players.size() << std::endl;
    //         for (auto &p : players) {
    //             auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
    //             pos->x = std::stof(action);
    //             pos->y = std::stof(msg.substr(msg.find(' ', msg.find(' ', 7) + 1) + 1));
    //             std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
    //         }
    //     }
    // }

    // void NetworkSystem::destroy() {}

    void NetworkSystem::onEntityAdded(std::shared_ptr<IEntity>) {}

    void NetworkSystem::onEntityRemoved(std::shared_ptr<IEntity>) {}

    // void NetworkSystem::writeMsg(const std::string &msg)
    // {
    //     if (Core::networkRole == NetworkRole::CLIENT) {
    //         _socket->write(msg, _serverAddr, _port);
    //     } else if (Core::networkRole == NetworkRole::SERVER) {
    //         for (auto &client : _senders)
    //             _socket->write(msg, QHostAddress(client.first), client.second);
    //     }
    // }

    // void NetworkSystem::putMsgInQueue(std::string msg)
    // {
    //     if (!msg.empty())
    //         _msgQueue.push_back(msg);

    //     QString addr = _socket->getLastAddress().toString();
    //     unsigned short port = _socket->getLastPort();
    //     for (auto &s : _senders) {
    //         if (s.first == addr && s.second == port)
    //             return;
    //     }
    //     _senders.push_back(std::make_pair(addr, port));

    //     if (msg == WAIT_CONNECTION && Core::networkRole == NetworkRole::SERVER) {
    //         _socket->write(CONNECTION_OK, QHostAddress(addr), port);
    //         // TODO: This isn't good
    //         emit clientConnection();
    //     }
    // }

}
/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkClientSystem.cpp
*/

#include "NetworkClientSystem.hpp"
#include "SceneManager.hpp"
#include "Core.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "GameSystem.hpp"

#include <regex>

namespace ecs
{

    NetworkClientSystem::NetworkClientSystem(std::string serverAddr, unsigned short port) : _timer(this)
    {
        _serverAddr = QHostAddress(QString(serverAddr.c_str()));
        _port = port;
        connect(&_timer, &QTimer::timeout, this, &NetworkClientSystem::onPingTimeout);
    }

    void NetworkClientSystem::destroy()
    {
        if (_connected)
            writeMsg(Message(NetworkMessageType::DISCONNECTED));
    }

    void NetworkClientSystem::init(SceneManager &)
    {
        std::cerr << "NetworkClientSystem::init" << std::endl;
        _socket = new UdpSocket(this, QHostAddress::AnyIPv4, 0);
        connect(_socket, &UdpSocket::transferMsgToSystem, this, &NetworkClientSystem::putMsgInQueue);
    }

    void NetworkClientSystem::update(SceneManager &manager, uint64_t dt)
    {
        static bool waitCo = false;

        if (manager.getCurrentSceneType() == SceneType::CONNECTION && !_connected && !waitCo) {
            writeMsg(Message(NetworkMessageType::WAIT_CONNECTION));
            waitCo = true;
        }

        for (auto &msg : _msgQueue) {
            if (msg.getMessageType() == MessageType::ENTITYMESSAGE)
                processEntityMessage(msg, manager, dt);
            else if (msg.getMessageType() == MessageType::TEXTMESSAGE && std::regex_match(msg.getText(), std::regex(std::string(RM_PLAYER) + " [1-4]"))) {
                removePlayer(msg.getText(), manager);
            } else if (msg.getMessageType() == MessageType::NETWORKEVENTMESSAGE) {
                if (waitCo && !_connected && msg.getNetworkMessageType() == NetworkMessageType::CONNECTION_OK) {
                    manager.setCurrentScene(SceneType::LOBBY);
                    waitCo = false;
                    _connected = true;
                    _timer.start(PING_TIMEOUT);
                } else if (msg.getNetworkMessageType() == NetworkMessageType::READY) {
                    manager.setCurrentScene(SceneType::GAME);
                }
            }
        }
        _msgQueue.clear();
    }

    void NetworkClientSystem::removePlayer(std::string s, SceneManager &sceneManager)
    {
        unsigned int id = std::stoi(s.erase(0, s.find(" ") + 1));

        for (auto &e : sceneManager.getCurrentScene()[{IEntity::Tags::PLAYER}]) {
            auto player = Component::castComponent<Player>((*e)[IComponent::Type::PLAYER]);
            if (player->getId() == id) {
                sceneManager.getCurrentScene().removeEntity(e);
                break;
            }
        }
    }

    void NetworkClientSystem::processEntityMessage(Message &message, SceneManager &sceneManager, uint64_t dt)
    {
        long unsigned int id = message.getEntityId();
        switch (message.getEntityAction()) {
            case EntityAction::CREATE:
                if (message.getEntityType() == EntityType::PLAYER) {
                    emit createPlayer(sceneManager.getScene(SceneType::GAME), KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL,
                    id, GameSystem::playerSpawns.front(), message.getIsMe());
                    GameSystem::playerSpawns.erase(GameSystem::playerSpawns.begin());
                } else if (message.getEntityType() == EntityType::MISSILE)
                    GameSystem::createMissile(sceneManager.getCurrentScene(), message.getEntityId(), Position(message.getEntityPosition().x, message.getEntityPosition().y), Missile::MissileType::PL_SIMPLE);
                break;
            case EntityAction::UPDATE:
                if (message.getEntityType() == EntityType::PLAYER)
                    handlePlayerEvent(sceneManager, message, dt);
                else if (message.getEntityType() == EntityType::MISSILE)
                    handleMissileUpdate(sceneManager, message, dt);
                break;
            default:
                break;
        }
    }

    void NetworkClientSystem::putMsgInQueue(Message msg)
    {
        _msgQueue.push_back(msg);
    }

    void NetworkClientSystem::writeMsg(const Message &msg)
    {
        _socket->write(msg, _serverAddr, _port);
        _timer.start(PING_TIMEOUT);
    }

    void NetworkClientSystem::onPingTimeout()
    {
        writeMsg(Message(NetworkMessageType::IMALIVE));
    }

    void NetworkClientSystem::handlePlayerEvent(SceneManager &manager, const Message &msg, uint64_t dt)
    {
        auto players = manager.getCurrentScene()[IEntity::Tags::PLAYER];

        for (auto &player : players) {
            if (player->getId() != msg.getEntityId())
                continue;
            auto pos = Component::castComponent<Position>((*player)[IComponent::Type::POSITION]);
            pos->x = msg.getEntityPosition().x;
            pos->y = msg.getEntityPosition().y;
            break;
        }
    }

    void NetworkClientSystem::handleMissileUpdate(SceneManager &sceneManager, const Message &msg, uint64_t dt)
    {
        auto missiles = sceneManager.getCurrentScene()[IEntity::Tags::MISSILE];

        for (auto &missile : missiles) {
            if (missile->getId() != msg.getEntityId())
                continue;
            auto pos = Component::castComponent<Position>((*missile)[IComponent::Type::POSITION]);
            pos->x = msg.getEntityPosition().x;
            pos->y = msg.getEntityPosition().y;
            break;
        }
    }

    void NetworkClientSystem::handlePlayerEvent(SceneManager &manager, std::string msg, uint64_t dt)
    {
        auto players = manager.getCurrentScene()[IEntity::Tags::PLAYER];

        msg.erase(0, 7);
        int playerId = std::stoi(msg.substr(0, msg.find(' ')));
        msg.erase(0, msg.find(' ') + 1);
        std::string comp = msg.substr(0, msg.find(' '));
        msg.erase(0, msg.find(' ') + 1);
        float x = std::stof(msg.substr(0, msg.find(' ')));
        msg.erase(0, msg.find(' ') + 1);
        float y = std::stof(msg);

        if (comp == "POS") {
            std::cerr << "POS: " << players.size() << std::endl;
            for (auto &p : players) {
                auto player = Component::castComponent<Player>((*p)[IComponent::Type::PLAYER]);
                if (player->getId() != playerId)
                    continue;
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                pos->x = x;
                pos->y = y;
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
                break;
            }
        }
    }

}
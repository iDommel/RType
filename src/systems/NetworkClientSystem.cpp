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

namespace ecs
{

    NetworkClientSystem::NetworkClientSystem() : _timer(this)
    {
        connect(&_timer, &QTimer::timeout, this, &NetworkClientSystem::onPingTimeout);
    }

    void NetworkClientSystem::destroy()
    {
        if (_connected)
            writeMsg(DISCONNECTED);
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
            writeMsg(WAIT_CONNECTION);
            waitCo = true;
        }

        for (auto &s : _msgQueue) {
            std::cerr << s << std::endl;
            if (waitCo && !_connected && s == CONNECTION_OK) {
                manager.setCurrentScene(SceneType::LOBBY);
                waitCo = false;
                _connected = true;
                _timer.start(PING_TIMEOUT);
            } else if (s == READY) {
                manager.setCurrentScene(SceneType::GAME);
            } else if (s.rfind(CR_PLAYER, 0) == 0) {
                int idPlayer = std::stoi(s.substr(std::string(CR_PLAYER).size()));
                emit createPlayer(manager.getScene(SceneType::GAME), KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL, idPlayer, GameSystem::_playerSpawns[idPlayer], false);
            } else if (s.rfind(CR_ME, 0) == 0) {
                int idPlayer = std::stoi(s.substr(std::string(CR_ME).size()));
                emit createPlayer(manager.getScene(SceneType::GAME), KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL, idPlayer, GameSystem::_playerSpawns[idPlayer], true);
            }
            if (s.rfind("PLAYER ", 0) == 0) {
                handlePlayerEvent(manager, s, dt);
            }
        }
        _msgQueue.clear();
    }

    void NetworkClientSystem::putMsgInQueue(std::string msg)
    {
        if (!msg.empty())
            _msgQueue.push_back(msg);
    }

    void NetworkClientSystem::writeMsg(const Message &msg)
    {
        _socket->write(msg, _serverAddr, _port);
        _timer.start(PING_TIMEOUT);
    }

    void NetworkClientSystem::onPingTimeout()
    {
        writeMsg(IMALIVE);
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
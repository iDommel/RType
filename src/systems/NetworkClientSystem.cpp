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
            } else if (s == READY) {
                manager.setCurrentScene(SceneType::GAME);
            } else if (s.rfind(CR_PLAYER, 0) == 0) {
                int idPlayer = std::stoi(s.substr(std::string(CR_PLAYER).size()));
                std::cerr << "Player " << idPlayer << std::endl;
                emit createPlayer(manager.getScene(SceneType::GAME), KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL, idPlayer, GameSystem::_playerSpawns[idPlayer]);
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

    void NetworkClientSystem::writeMsg(const std::string &msg)
    {
        // std::cerr << "write: " << msg << std::endl;
        _socket->write(msg, _serverAddr, _port);
    }

    void NetworkClientSystem::handlePlayerEvent(SceneManager &manager, const std::string &msg, uint64_t dt)
    {
        auto players = manager.getCurrentScene()[IEntity::Tags::PLAYER];
        std::string axis = msg.substr(7, msg.find(' ', 7) - 7);
        std::string action = msg.substr(msg.find(' ', 7) + 1, msg.find(' ', msg.find(' ', 7) + 1) - msg.find(' ', 7) - 1);

        if (axis == "POS") {
            std::cerr << "POS: " << players.size() << std::endl;
            for (auto &p : players) {
                auto pos = Component::castComponent<Position>((*p)[IComponent::Type::POSITION]);
                pos->x = std::stof(action);
                pos->y = std::stof(msg.substr(msg.find(' ', msg.find(' ', 7) + 1) + 1));
                std::cerr << "Player pos: " << pos->x << ", " << pos->y << std::endl;
            }
        }
    }

}
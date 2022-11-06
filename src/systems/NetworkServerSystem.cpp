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
            if (s.getMessageType() == MessageType::NETWORKEVENTMESSAGE) {
                if (s.getNetworkMessageType() == NetworkMessageType::DISCONNECTED)
                    deconnectClient(s.getSender());
                else if (s.getNetworkMessageType() == NetworkMessageType::WAIT_CONNECTION && manager.getCurrentSceneType() == SceneType::LOBBY) {
                    connectClient(s.getSender());
                } else if (s.getNetworkMessageType() == NetworkMessageType::READY)
                    setClientReady(s.getSender(), manager);
            }
            if (s.getEventType() == EventType::KEYBOARD) {
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
            if (keyState == KeyState::DOWN || keyState == KeyState::PRESSED)
                playerComp->moveRight(manager, entity, dt);
            else
                playerComp->stopRight(manager, entity, dt);
            break;
        case KEY_LEFT:
            if (keyState == KeyState::DOWN || keyState == KeyState::PRESSED)
                playerComp->moveLeft(manager, entity, dt);
            else
                playerComp->stopLeft(manager, entity, dt);
            break;
        case KEY_UP:
            if (keyState == KeyState::DOWN || keyState == KeyState::PRESSED)
                playerComp->moveUp(manager, entity, dt);
            else
                playerComp->stopUp(manager, entity, dt);
            break;
        case KEY_DOWN:
            if (keyState == KeyState::DOWN || keyState == KeyState::PRESSED)
                playerComp->moveDown(manager, entity, dt);
            else
                playerComp->stopDown(manager, entity, dt);
            break;
        case KEY_RIGHT_CONTROL:
            if (keyState == KeyState::PRESSED)
                playerComp->startClock();
            else if (keyState == KeyState::RELEASED) {
                // playerComp->shootMissile(manager, entity, dt);
                if (playerComp->getShootTimer().msecsTo(QTime::currentTime()) > 1000)
                    return;
                Vector2 missilePos = {pos->x + 64, pos->y + 32};
                GameSystem::createSimpleMissile(manager.getCurrentScene(), Entity::idCounter, Position(missilePos.x, missilePos.y));
                Message msg(EntityAction::CREATE, Entity::idCounter++, EntityType::MISSILE, missilePos);
                writeMsg(msg);
            }
            return;
        default:
            return;
        }
        Message response(EntityAction::UPDATE, (uint64_t)id, EntityType::PLAYER, pos->getVector2());
        writeMsg(response);
    }

    void NetworkServerSystem::writeMsg(const Message &msg)
    {
        for (auto &client : _senders)
            _socket->write(msg, QHostAddress(client.first), client.second);
    }

    void NetworkServerSystem::writeToClient(Message msg, int clientId)
    {
        _socket->write(msg, QHostAddress(_senders[clientId].first), _senders[clientId].second);
    }

    void NetworkServerSystem::writeToClient(Message msg, std::pair<QString /*addr*/, unsigned short /*port*/> client)
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
        _socket->write(Message(NetworkMessageType::CONNECTION_OK), QHostAddress(client.first), client.second);
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
            writeMsg(Message(EntityAction::DELETE, (uint64_t)id));
    }

    void NetworkServerSystem::setClientReady(std::pair<QString /*addr*/, unsigned short /*port*/> client, SceneManager &manager)
    {
        if (_states[client] != ClientState::CONNECTED)
            return;
        _states[client] = ClientState::READYTOPLAY;

        // check if all players are ready
        for (auto s : _states) {
            if (s.second != ClientState::READYTOPLAY)
                return;
        }

        // Create players inside clients
        for (auto &client : _senders) {
            _playersId[client] = Entity::idCounter++;
            unsigned long int id = _playersId[client];
            emit createPlayer(manager.getScene(SceneType::GAME), KEY_Q, KEY_D, KEY_Z, KEY_S, KEY_RIGHT_CONTROL, id, GameSystem::playerSpawns.front(), false);
            GameSystem::playerSpawns.erase(GameSystem::playerSpawns.begin());
            for (auto &player : _senders) {
                Message msg(EntityAction::CREATE, id, EntityType::PLAYER, (client == player));
                std::cout << msg.getIsMe() << std::endl;
                writeToClient(msg, player);
            }
        }

        // notify clients game can start
        writeMsg(Message(NetworkMessageType::READY));
        emit changeScene(SceneType::GAME);
    }

}
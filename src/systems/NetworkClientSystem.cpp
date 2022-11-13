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
            if (msg.getMessageType() == MessageType::ENTITY_MESSAGE)
                processEntityMessage(msg, manager, dt);
            else if (msg.getMessageType() == MessageType::NETWORK_EVENT_MESSAGE) {
                if (waitCo && !_connected && msg.getNetworkMessageType() == NetworkMessageType::CONNECTION_OK) {
                    manager.setCurrentScene(SceneType::LOBBY);
                    waitCo = false;
                    _connected = true;
                    _timer.start(PING_TIMEOUT);
                } else if (msg.getNetworkMessageType() == NetworkMessageType::READY) {
                    manager.setCurrentScene(SceneType::LEVEL_1);
                }

            } else if (msg.getMessageType() == MessageType::SCENE_CHANGE_MESSAGE && msg.getSceneType() != -1) {
                manager.setCurrentScene((SceneType)msg.getSceneType());
            }
        }
        _msgQueue.clear();
    }

    void NetworkClientSystem::removePlayer(QUuid id, SceneManager &sceneManager)
    {
        for (auto &player : sceneManager.getCurrentScene()[IEntity::Tags::PLAYER]) {
            if (player->getId() == id) {
                sceneManager.getCurrentScene().removeEntity(player);
                break;
            }
        }
        return;
    }

    void NetworkClientSystem::removeEntity(QUuid id, SceneManager &sceneManager)
    {
        auto entity = sceneManager.getCurrentScene().getEntityById(id);
        if (entity) {
            sceneManager.getCurrentScene().removeEntity(entity);
        }
        return;
    }

    void NetworkClientSystem::processEntityMessage(Message &message, SceneManager &sceneManager, uint64_t dt)
    {
        QUuid id = message.getEntityId();
        std::shared_ptr<IEntity> e = sceneManager.getCurrentScene().getEntityById(id);

        switch (message.getEntityAction()) {
        case EntityAction::CREATE:
            if (message.getEntityType() == EntityType::PLAYER) {
                emit createPlayer(sceneManager.getScene(SceneType::LEVEL_1), KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT_CONTROL, KEY_SPACE,
                                  id, Position(message.getEntityPosition()), bool(message.getArg()));
            } else if (message.getEntityType() == EntityType::MISSILE) {
                GameSystem::createMissile(sceneManager, message.getEntityId(), Position(message.getEntityPosition()), Missile::MissileType(message.getArg()));
            } else if (message.getEntityType() == EntityType::ENEMY) {
                Vector2 pos = message.getEntityPosition();
                GameSystem::createEnemy(sceneManager.getScene(SceneType::LEVEL_1), Enemy::EnemyType(message.getArg()), pos.x, pos.y, message.getEntityId());
            } else if (message.getEntityType() == EntityType::MODULE)
                GameSystem::createSpaceModule(sceneManager, message.getEntityId(), Position(message.getEntityPosition()), message.getArg());
            else if (message.getEntityType() == EntityType::BONUS)
                sceneManager.getCurrentScene().addEntity(GameSystem::createBonus(message.getEntityId(), Position(message.getEntityPosition())));
            break;
        case EntityAction::UPDATE:
            if (message.getEntityType() == EntityType::PLAYER)
                handlePlayerEvent(sceneManager, message, dt);
            else if (message.getEntityType() == EntityType::MISSILE)
                handleMissileUpdate(sceneManager, message, dt);
            else if (message.getEntityType() == EntityType::ENEMY)
                handleEnemyUpdate(sceneManager, message, dt);
            else if (message.getEntityType() == EntityType::MODULE)
                handleSpaceModuleUpdate(sceneManager, message, dt);
            break;
        case EntityAction::DELETE:
            removeEntity(id, sceneManager);
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

    void NetworkClientSystem::handleEnemyUpdate(SceneManager &sceneManager, const Message &msg, uint64_t dt)
    {
        auto enemies = sceneManager.getCurrentScene()[IEntity::Tags::ENEMY];

        for (auto &enemy : enemies) {
            if (enemy->getId() != msg.getEntityId())
                continue;
            auto pos = Component::castComponent<Position>((*enemy)[IComponent::Type::POSITION]);
            pos->x = msg.getEntityPosition().x;
            pos->y = msg.getEntityPosition().y;
            break;
        }
    }

    void NetworkClientSystem::handleSpaceModuleUpdate(SceneManager &manager, const Message &msg, uint64_t dt)
    {
        for (auto &mod : manager.getCurrentScene()[IEntity::Tags::SPACE_MODULE]) {
            if (mod->getId() != msg.getEntityId())
                continue;
            auto pos = Component::castComponent<Position>((*mod)[IComponent::Type::POSITION]);
            pos->x = msg.getEntityPosition().x;
            pos->y = msg.getEntityPosition().y;
            break;
        }
    }
}
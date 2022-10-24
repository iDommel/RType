/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** ANetworkSystem.cpp
*/

#include "ANetworkSystem.hpp"
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

    ANetworkSystem::ANetworkSystem() : QObject(nullptr)
    {
        _serverAddr = QHostAddress(QStringLiteral("172.18.0.1"));
        _port = 8080;
        if (Core::networkRole == NetworkRole::UNDEFINED)
            throw std::invalid_argument("NetworkRole undefined");
    }

    void ANetworkSystem::onEntityAdded(std::shared_ptr<IEntity>, SceneType) {}

    void ANetworkSystem::onEntityRemoved(std::shared_ptr<IEntity>) {}

}
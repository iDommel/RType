/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkSystem.cpp
*/

#include "NetworkSystem.hpp"
#include <QtNetwork>

namespace ecs
{
    QHostAddress NetworkSystem::_serverAddr = QHostAddress::LocalHost;
    const unsigned short NetworkSystem::_port = 8080;

NetworkSystem::NetworkSystem(NetworkRole role) : QObject(nullptr), _role(role)
{
    connect(this, &NetworkSystem::sendMessage, this, &NetworkSystem::writeMsg);

    _groupAddr = QHostAddress(QStringLiteral("239.255.43.21"));
    if (_role == NetworkRole::UNDEFINED)
        throw std::invalid_argument("NetworkRole undefined");
}

void NetworkSystem::init(SceneManager &)
{
    if (_role == NetworkRole::CLIENT) {
        _socket = new UdpSocket(this, QHostAddress::AnyIPv4, _port, QUdpSocket::ShareAddress);
        _socket->joinMulticastGroup(_groupAddr);
    } else if (_role == NetworkRole::SERVER) {
        _socket = new UdpSocket(this, _serverAddr, _port);
    }
}

void NetworkSystem::update(SceneManager &, uint64_t)
{
    /*
    Treat all the elements in the list of messages

    if (role == CLIENT)
        correct game entities

    else if (role == SERVER)
        correct game entities & send modifications to clients
    */
}

void NetworkSystem::destroy() {}

void NetworkSystem::onEntityAdded(std::shared_ptr<IEntity>) {}

void NetworkSystem::onEntityRemoved(std::shared_ptr<IEntity>) {}

void NetworkSystem::writeMsg(std::string msg)
{
    if (_role == NetworkRole::CLIENT)
        _socket->write(msg, _serverAddr, _port);
    else if (_role == NetworkRole::SERVER)
        _socket->write(msg, _groupAddr, _port);
}

}
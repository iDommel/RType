/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkSystem.cpp
*/

#include "NetworkSystem.hpp"
#include <QtNetwork>

namespace indie
{
    QHostAddress _serverAddr;
    const QHostAddress _groupAddr;
    const unsigned short _port;

NetworkSystem::NetworkSystem(NetworkRole role) : _role(role)
{
    if (_role == UNDEFINED)
        throw std::invalid_argument("NetworkRole undefined");
}

void NetworkSystem::init(SceneManager &)
{
    if (_role == NetworkRole::CLIENT) {
        _socket = new UdpSocket(this, QHostAddress::AnyIPv4, _port, QUdpSocket::ShareAddress);
        _socket->joinMulticastGroup(_groupAddr);
    } else if (role == NetworkRole::SERVER) {
        _socket = new UdpSocket(this, QHostAddress::AnyIPv4, _port);
    }
}

void NetworkSystem::~NetworkSystem()
{
    _listeningThread.quit();
    _listeningThread.wait();
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

}
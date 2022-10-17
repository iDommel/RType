/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkSystem.cpp
*/

#include "NetworkSystem.hpp"
#include <QtNetwork>
#include <iostream>

namespace ecs
{

NetworkSystem::NetworkSystem(NetworkRole role) : QObject(nullptr), _role(role)
{
    _serverAddr = QHostAddress::LocalHost;
    _port = 8080;
    if (_role == NetworkRole::UNDEFINED)
        throw std::invalid_argument("NetworkRole undefined");
}

void NetworkSystem::init(SceneManager &)
{
    std::cerr << "NetworkSystem::init" << std::endl;
    if (_role == NetworkRole::CLIENT) {
        _socket = new UdpSocket(this, QHostAddress::AnyIPv4, 0);
        writeMsg(WAIT_CONNECTION);
        std::cerr << "Waiting for connection..." << std::endl;
        if (!_socket->waitForServerConnection())
            throw std::runtime_error("Connection to server failed");
    } else if (_role == NetworkRole::SERVER) {
        _socket = new UdpSocket(this, _serverAddr, _port);
    }
}

void NetworkSystem::update(SceneManager &, uint64_t)
{
    // std::cerr << "NetworkSystem::update" << std::endl;
    // if (_socket->canRead()) {
    //     if (_role == NetworkRole::SERVER) {
    //         if (_socket->readDatagram() == WAIT_CONNECTION) {
    //             std::cerr << "Client connected: " << _socket->getLastAddress().toString().toStdString() << ":" << _socket->getLastPort() << std::endl;
    //             writeMsg(CONNECTION_OK);
    //         }
    //     } else
    //         _socket->readDatagrams();
    // }
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

void NetworkSystem::writeMsg(const std::string &msg)
{
    if (_role == NetworkRole::CLIENT) {
        std::cerr << "write to server" << std::endl;
        _socket->write(msg, _serverAddr, _port);
    } else if (_role == NetworkRole::SERVER) {
        std::cerr << "Write to clients" << std::endl;
        _socket->write(msg, _socket->getLastAddress(), _socket->getLastPort());
    }
}

}
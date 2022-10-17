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
    } else if (_role == NetworkRole::SERVER) {
        _socket = new UdpSocket(this, _serverAddr, _port);
    }

    connect(_socket, &UdpSocket::transferMsgToSystem, this, &NetworkSystem::putMsgInQueue);
}

void NetworkSystem::update(SceneManager &manager, uint64_t)
{
    static bool waiting = false;
    // std::cerr << "NetworkSystem::update" << std::endl;
    // if (_socket->canRead()) {
    //     if (_role == NetworkRole::SERVER) {
    //         if (_socket->readDatagram() == WAIT_CONNECTION) {
    //             std::cerr << "Client connected: " << _socket->getLastAddress().toString().toStdString() << ":" << _socket->getLastPort() << std::endl;
    //             writeMsg(CONNECTION_OK);
    //         }
    //     } else {
    //         if (_socket->readDatagram() == WAIT_CONNECTION
    //         _socket->readDatagrams();
    //     }
    // }
    /*
    Treat all the elements in the list of messages

    if (role == CLIENT)
        correct game entities

    else if (role == SERVER)
        correct game entities & send modifications to clients
    */
   if (manager.getCurrentSceneType() == SceneManager::SceneType::CONNECTION && _role == NetworkRole::CLIENT && !waiting) {
        writeMsg(WAIT_CONNECTION);
        waiting = true;
   }

   for (auto &s : _msgQueue) {
       std::cerr << s << std::endl;
       if (waiting && s == CONNECTION_OK && _role == NetworkRole::CLIENT) {
           emit clientConnection();
           waiting = false;
       }
   }
   _msgQueue.clear();
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
        for (auto &client : _senders)
            _socket->write(msg, QHostAddress(client.first), client.second);
    }
}

void NetworkSystem::putMsgInQueue(std::string msg)
{
    if (!msg.empty())
        _msgQueue.push_back(msg);

    QString addr = _socket->getLastAddress().toString();
    unsigned short port = _socket->getLastPort();
    for (auto &s : _senders) {
        if (s.first == addr && s.second == port)
            return;
    }
    _senders.push_back(std::make_pair(addr, port));
    // if (_senders.find(_socket->getLastAddress().toString()) != _senders.end()) {
    //     return;
    // }
    // _senders[_socket->getLastAddress().toString()] = _socket->getLastPort();
    // if (msg == CONNECTION_OK && _role == NetworkRole::CLIENT)
    //     std::cerr << "Connected to server." << std::endl;
    if (msg == WAIT_CONNECTION && _role == NetworkRole::SERVER)
        _socket->write(CONNECTION_OK, QHostAddress(addr), port);
}

}
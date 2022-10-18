/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** UdpSocket.cpp
*/

#include "UdpSocket.hpp"
#include <iostream>

namespace ecs
{

UdpSocket::UdpSocket(QObject *parent, QHostAddress address, int port, QAbstractSocket::BindMode mode) : QObject(parent)
{
    _socket = new QUdpSocket(this);
    _socket->bind(address, port, mode);
    if (!connect(_socket, &QUdpSocket::readyRead, this, &UdpSocket::readDatagrams))
        throw std::runtime_error("UDP socket: Couldn't connect");
}

void UdpSocket::joinMulticastGroup(QHostAddress groupAddress)
{
    if (!_socket->joinMulticastGroup(groupAddress))
        throw std::runtime_error("Couldn't join multicast group: " + _socket->errorString().toStdString());
}

void UdpSocket::readDatagrams()
{
    while (_socket->hasPendingDatagrams()) {
        readDatagram();
    }
}

void UdpSocket::readDatagram()
{
    if (!_socket->hasPendingDatagrams())
        return;
    QNetworkDatagram data = _socket->receiveDatagram();
    _lastPort = data.senderPort();
    _lastAddr = data.senderAddress();
    std::cerr << "Message received: " << data.data().toStdString() << std::endl;
    emit transferMsgToSystem(data.data().toStdString());
}

bool UdpSocket::canRead()
{
    return _socket->hasPendingDatagrams();
}

bool UdpSocket::waitReadyRead(int ms)
{
    return _socket->waitForReadyRead(ms);
}

void UdpSocket::write(const std::string &msg, const QHostAddress &address, int port)
{
    QByteArray buffer(msg.c_str(), msg.size());
    QNetworkDatagram data(buffer, address, port);
    if (_socket->writeDatagram(data) == -1) {
        throw std::runtime_error("Error sending msg: " + msg);
    }
    std::cerr << "Send msg: " << msg << std::endl;
}

}

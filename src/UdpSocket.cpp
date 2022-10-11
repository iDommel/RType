/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** UdpSocket.cpp
*/

#include "UdpSocket.hpp"
#include <iostream>

namespace indie
{

UdpSocket::UdpSocket(QObject *parent, QHostAddress address, int port, QAbstractSocket::BindMode mode) : QObject(parent), _socket(parent)
{
    _socket.bind(address, port, mode);
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(readDatagrams()));
}

void UdpSocket::joinMulticastGroup(QHostAddress groupAddress)
{
    if (!_socket.joinMulticastGroup(groupAddress))
        throw runtime_error("Couldn't join multicast group: " + _socket.errorString().toStdString());
}

void Socket::readDatagrams()
{
    while (_socket.hasPendingDatagrams()) {
        QNetworkDatagram data = _socket.receiveDatagram();
        // process data
        std::cout << data.data().toStdString() << std::endl;
    }
}

void UdpSocket::write(const std::string &msg, const QHostAddress &address, int port)
{
    QByteArray buffer(msg.c_str(), msg.size());
    QNetworkDatagram data(buffer, address, port);
    if (_socket.writeDatagram(data) == -1) {
        throw std::runtime_error("Error sending msg: " + msg);
    }
    std::cout << "Send msg: " << msg << std::endl;
}

bool UdpSocket::canRead()
{
    if (_socket.hasPendingDatagrams()) {
        emit _socket.readyRead();
        return true;
    }
    return false;
}

}

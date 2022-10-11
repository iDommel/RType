/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Socket.hpp
*/

#ifndef UDP_SOCKET_HPP
#define UDP_SOCKET_HPP

#include <QtNetwork>
#include <string>

namespace ecs
{

    class UdpSocket : public QObject
    {
        Q_OBJECT

    public:
        /**
         * @brief Construct a new UDP socket
         * @param parent Parent object
         * @param address Address to bind to
         * @param port Port to bind to
         * @param mode Bind mode
         */
        UdpSocket(QObject *parent = nullptr, QHostAddress address = QHostAddress::AnyIPv4, \
            int port = 0, QAbstractSocket::BindMode mode = QAbstractSocket::DefaultForPlatform);

        /// @brief Joins the multicast group on the default interface chosen by the operating system.
        /// @param groupAddress Address of the group to join
        void joinMulticastGroup(QHostAddress groupAddress);

        void write(const std::string &msg, const QHostAddress &address, int port);

    private slots:
        void readDatagrams();

    private:
        QUdpSocket _socket;

    };

}

#endif /* UDP_SOCKET_HPP */
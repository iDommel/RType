/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Socket.hpp
*/

#ifndef UDP_SOCKET_HPP
#define UDP_SOCKET_HPP

#define WAIT_CONNECTION     "WAIT"
#define CONNECTION_OK       "OK"
#define DISCONNECTED         "STOP"
#define READY               "READY"

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

        /// @brief Writes a message
        /// @param msg Message to send
        /// @param address Address to send message to
        /// @param port Port to send message to
        void write(const std::string &msg, const QHostAddress &address, int port);

        /// @brief Tells if datagrams are available
        /// @return Returns True if the queue is not empty, false otherwise
        bool canRead();

        bool waitReadyRead(int ms = 30000);

        /// @brief Returns the address of the last sender
        QHostAddress getLastAddress() { return _lastAddr; };

        /// @brief Returns the port of the last sender
        unsigned short getLastPort() { return _lastPort; };

    public slots:
        /// @brief Reads all pending messages
        void readDatagrams();
        /// @brief Reads a message from the queue
        void readDatagram();

    signals:
        /// @brief Transferts the received message to the ANetworkSystem
        /// @param msg Massage to be transfered
        void transferMsgToSystem(std::string msg);

    private:
        QUdpSocket *_socket;
        QHostAddress _lastAddr;
        unsigned short _lastPort = 0;

    };

}

#endif /* UDP_SOCKET_HPP */
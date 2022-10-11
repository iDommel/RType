/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** NetworkSystem.hpp
*/

#ifndef NETWORK_SYSTEM_HPP
#define NETWORK_SYSTEM_HPP

#include <map>

#include "ISystem.hpp"
#include "UdpSocket.hpp"
#include <QtCore>

class QHostAddress;
class QThread;

namespace rtype
{

    class NetworkSystem : public ISystem, public QObject
    {

        Q_OBJECT

    public:

        enum class NetworkRole {
            UNDEFINED = -1,
            SERVER,
            CLIENT
        };

        NetworkSystem(NetworkRole role);
        ~NetworkSystem();

        void init(SceneManager &manager) final;
        void update(SceneManager &manager, uint64_t deltaTime) final;
        void destroy() final;

        /**
         * @brief The callback to be called when an entity is added to a scene
         * @param entity The Entity that was added
         */
        void loadEntity(std::shared_ptr<IEntity> entity) final {};
        /**
         * @brief The callback to be called when an entity is removed from a scene
         * @param entity The Entity that was removed
         */
        void unloadEntity(std::shared_ptr<IEntity> entity) final {};

    private:
        static QHostAddress _serverAddr;
        static const QHostAddress _groupAddr = QStringLiteral("239.255.43.21");
        static const unsigned short _port = 8080;

        NetworkRole _role = UNDEFINED;
        UdpSocket *_socket;
    };

}

#endif /* NETWORK_SYSTEM_HPP */
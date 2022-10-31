/*
** EPITECH PROJECT, 2022
** test
** File description:
** Message
*/

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_
#include <QtGlobal>
#include <QString>
#include <string>
#include "raylib.h"
namespace ecs
{
    template <typename E>
    constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
    {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

    enum class MessageType : qint8 {
        UNDEFINED = -1,
        ENTITYMESSAGE,
        GRAPHICEVENTMESSAGE,
        NETWORKEVENTMESSAGE,
        TEXTMESSAGE,
    };

    enum class EventType : qint8 {
        UNDEFINED = -1,
        KEYBOARD,
        MOUSE,
    };

    enum class EntityAction : qint8 {
        UNDEFINED = -1,
        CREATE,
        UPDATE,
        DELETE,
    };

    enum class EntityType : qint8 {
        UNDEFINED = -1,
        PLAYER,
        ENEMY,
        BULLET,
    };

    enum class KeyState : qint8 {
        UNDEFINED = -1,
        PRESSED,
        RELEASED,
        UP,
        DOWN
    };

    enum class CustomMouseButton : qint8 {
        UNDEFINED = -1,
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE,
    };

    class Message
    {
    public:
        Message();
        /// @brief Constructor for TEXTMESSAGE messages
        /// @param str the string to send
        Message(const std::string &);
        /// @brief Alternate constructor for TEXTMESSAGE messages
        /// @param str the string to send
        Message(const char *);
        /// @brief Constructor for KeyboardEvents messages
        /// @param eventType the type of the event
        /// @param keyState the state of the key pressed
        /// @param key the key pressed (raylib enum)
        Message(EventType, KeyState, KeyboardKey);

        /// @brief Constructor for MouseEvents messages
        /// @param eventType the type of the event
        /// @param keyState the state of the key pressed
        /// @param mouseButton the mouse button pressed (our own enum)
        Message(EventType, KeyState, CustomMouseButton);

        /// @brief Constructor for deleting an Entity
        /// @param action, the action to do, should be DELETE
        /// @param id the id of the entity to delete
        Message(EntityAction, uint64_t);

        /// @brief Constructor for creating an Entity
        /// @param action, the action to do, should be CREATE
        /// @param id the id of the entity to create
        /// @param type the type of the entity to create
        Message(EntityAction, uint64_t, EntityType);

        /// @brief Constructor for updating an Entity
        /// @param action, the action to do, should be UPDATE
        /// @param id the id of the entity to update
        /// @param type the type of the entity to update
        /// @param pos the new position of the entity
        Message(EntityAction, uint64_t, EntityType, Vector2);

        ~Message();
        friend QDataStream &operator<<(QDataStream &out, const Message &toWrite);
        friend QDataStream &operator>>(QDataStream &in, Message &toWrite);
        void print() const;
        std::string toString() const;

    protected:
    private:
        friend QDataStream &readGraphicEventMessage(QDataStream &in, Message &toWrite);
        friend QDataStream &writeGraphicEventMessage(QDataStream &in, const Message &toWrite);
        friend QDataStream &readEntityMessage(QDataStream &in, Message &toWrite);
        friend QDataStream &writeEntityMessage(QDataStream &in, const Message &toWrite);

        MessageType _messageType = MessageType::UNDEFINED;
        EventType _eventType = EventType::UNDEFINED;
        EntityAction _entityAction = EntityAction::UNDEFINED;
        EntityType _entityType = EntityType::UNDEFINED;
        KeyboardKey _keyboardKey = KeyboardKey::KEY_NULL;
        CustomMouseButton _mouseButton = CustomMouseButton::UNDEFINED;
        KeyState _keyState = KeyState::UNDEFINED;
        Vector2 _mousePosition = {-1, -1};
        Vector2 _pos = {-1, -1};
        QString _textMessage = "";
        quint64 _id = 0;
    };
}

#endif /* !MESSAGE_HPP_ */

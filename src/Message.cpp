/*
** EPITECH PROJECT, 2022
** test
** File description:
** Message
*/

#include "Message.hpp"
#include <QDataStream>
#include <iostream>
#include <sstream>
namespace ecs
{

    Message::Message(const std::string &text)
        : _messageType(MessageType::TEXTMESSAGE), _textMessage(QString::fromStdString(text))
    {
    }

    Message::Message(const char *str)
        : _messageType(MessageType::TEXTMESSAGE), _textMessage(QString(str))
    {
    }

    Message::Message(EventType eventType, KeyState keyState, KeyboardKey key)
        : _messageType(MessageType::GRAPHICEVENTMESSAGE), _eventType(eventType), _keyState(keyState), _keyboardKey(key)
    {
    }

    Message::Message(EventType eventType, KeyState keyState, CustomMouseButton mouseButton)
        : _messageType(MessageType::GRAPHICEVENTMESSAGE), _eventType(eventType), _keyState(keyState), _mouseButton(mouseButton)
    {
    }

    Message::Message(EntityAction action, uint64_t id, EntityType type, Vector2 pos)
        : _messageType(MessageType::ENTITYMESSAGE), _entityAction(action), _entityType(type), _id(id), _pos(pos)
    {
        if (action != EntityAction::UPDATE)
            throw std::runtime_error("Wrong constructor for this action");
    }

    Message::Message(EntityAction action, uint64_t id, EntityType type)
        : _messageType(MessageType::ENTITYMESSAGE), _entityAction(action), _entityType(type), _id(id)
    {
        if (action != EntityAction::CREATE)
            throw std::runtime_error("Wrong constructor for this action");
    }

    Message::Message(EntityAction action, uint64_t id)
        : _messageType(MessageType::ENTITYMESSAGE), _entityAction(action), _id(id)
    {
        if (action != EntityAction::DELETE)
            throw std::runtime_error("Wrong constructor for this action");
    }

    Message::~Message()
    {
    }

    QDataStream &readGraphicEventMessage(QDataStream &in, Message &toWrite)
    {
        qint8 eventType;
        qint8 keyState;

        in >> eventType;
        in >> keyState;
        toWrite._eventType = static_cast<EventType>(eventType);
        toWrite._keyState = static_cast<KeyState>(keyState);
        switch (toWrite._eventType) {
        case EventType::KEYBOARD:
            quint32 key;
            in >> key;
            toWrite._keyboardKey = static_cast<KeyboardKey>(key);
            break;
        case EventType::MOUSE:
            qint8 mouseButton;
            in >> mouseButton;
            toWrite._mouseButton = static_cast<CustomMouseButton>(mouseButton);
            // in >> toWrite._mousePos;
            break;
        default:
            break;
        }
        return in;
    }

    QDataStream &readEntityMessage(QDataStream &in, Message &toWrite)
    {
        qint8 entityAction;
        qint8 entityType;
        in >> entityAction;
        toWrite._entityAction = static_cast<EntityAction>(entityAction);

        switch (toWrite._entityAction) {
        case EntityAction::CREATE:
            in >> entityType;
            toWrite._entityType = static_cast<EntityType>(entityType);
            break;
        case EntityAction::UPDATE:
            in >> entityType;
            toWrite._entityType = static_cast<EntityType>(entityType);
            break;
        case EntityAction::DELETE:
            break;
        default:
            break;
        }
        return in;
    }

    QDataStream &operator>>(QDataStream &in, Message &toWrite)
    {
        qint8 messageType;

        in >> messageType;
        toWrite._messageType = static_cast<MessageType>(messageType);
        switch (toWrite._messageType) {
            {
            case MessageType::GRAPHICEVENTMESSAGE:
                readGraphicEventMessage(in, toWrite);
                break;
            case MessageType::ENTITYMESSAGE:
                readEntityMessage(in, toWrite);
                break;
            case MessageType::NETWORKEVENTMESSAGE:
                break;
            case MessageType::TEXTMESSAGE:
                in >> toWrite._textMessage;
                break;
            default:
                break;
            }
        }
        return in;
    }

    QDataStream &writeGraphicEventMessage(QDataStream &out, const Message &toWrite)
    {
        out << to_integral(toWrite._eventType);
        out << to_integral(toWrite._keyState);
        switch (toWrite._eventType) {
        case EventType::KEYBOARD:
            out << to_integral(toWrite._keyboardKey);
            break;
        case EventType::MOUSE:
            out << to_integral(toWrite._mouseButton);
            // out << toWrite._mousePos;
            break;
        default:
            break;
        }
        return out;
    }

    QDataStream &writeEntityMessage(QDataStream &out, const Message &toWrite)
    {
        out << to_integral(toWrite._entityAction);
        switch (toWrite._entityAction) {
        case EntityAction::CREATE:
            out << to_integral(toWrite._entityType);
            break;
        case EntityAction::UPDATE:
            out << to_integral(toWrite._entityType);
            break;
        case EntityAction::DELETE:
            break;
        default:
            break;
        }
        return out;
    }

    QDataStream &operator<<(QDataStream &out, const Message &toWrite)
    {
        out << to_integral(toWrite._messageType);
        switch (toWrite._messageType) {
        case MessageType::GRAPHICEVENTMESSAGE:
            writeGraphicEventMessage(out, toWrite);
            break;
        case MessageType::ENTITYMESSAGE:
            writeEntityMessage(out, toWrite);
            break;
        case MessageType::NETWORKEVENTMESSAGE:
            break;
        case MessageType::TEXTMESSAGE:
            out << toWrite._textMessage;
            break;
        default:
            break;
        }
        return out;
    }

    void Message::print() const
    {
        std::cout << toString();
    }

    std::string Message::toString() const
    {
        std::stringstream ss;
        ss << "Message type: " << std::to_string(to_integral(_messageType)) << std::endl;
        switch (_messageType) {
        case MessageType::GRAPHICEVENTMESSAGE:
            ss << "Event type: " << std::to_string(to_integral(_eventType)) << std::endl;
            ss << "Key state: " << std::to_string(to_integral(_keyState)) << std::endl;
            switch (_eventType) {
            case EventType::KEYBOARD:
                ss << "Key: " << std::to_string(to_integral(_keyboardKey)) << std::endl;
                break;
            case EventType::MOUSE:
                ss << "Mouse button: " << std::to_string(to_integral(_mouseButton)) << std::endl;
                // ss << "Mouse pos: " << _mousePos << std::endl;
                break;
            default:
                break;
            }
            break;
        case MessageType::ENTITYMESSAGE:
            ss << "Entity action: " << std::to_string(to_integral(_entityAction)) << std::endl;
            switch (_entityAction) {
            case EntityAction::CREATE:
                ss << "Entity type: " << std::to_string(to_integral(_entityType)) << std::endl;
                break;
            case EntityAction::UPDATE:
                ss << "Entity type: " << std::to_string(to_integral(_entityType)) << std::endl;
                break;
            case EntityAction::DELETE:
                break;
            default:
                break;
            }
            break;
        case MessageType::NETWORKEVENTMESSAGE:
            break;
        case MessageType::TEXTMESSAGE:
            ss << "Text: " << _textMessage.toStdString() << std::endl;
            break;
        default:
            break;
        }
        return ss.str();
    }
}  // namespace ecs
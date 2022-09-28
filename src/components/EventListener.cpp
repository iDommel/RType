/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** EventListener
*/

#include "raylib.h"

#include "EventListener.hpp"

namespace indie
{

    EventListener::EventListener() : Component(Type::EVT_LISTENER)
    {
        _isInitialized = true;
    }

    EventListener::~EventListener()
    {
    }

    void EventListener::addKeyboardEvent(KeyboardKey key, ButtonCallbacks callbacks)
    {
        _keyboardMap[key] = callbacks;
    }

    void EventListener::removeKeyboardEvent(KeyboardKey key)
    {
        _keyboardMap.erase(key);
    }

    void EventListener::replaceKeyboardEvent(KeyboardKey oldKey, KeyboardKey newKey)
    {
        _keyboardMap[newKey] = _keyboardMap[oldKey];
        _keyboardMap.erase(oldKey);
    }

    void EventListener::addMouseEvent(MouseButton key, MouseCallbacks func)
    {
        _mouseMap[key] = func;
    }

    void EventListener::removeMouseEvent(MouseButton button)
    {
        _mouseMap.erase(button);
    }

    void EventListener::replaceMouseEvent(MouseButton oldButton, MouseButton newButton)
    {
        _mouseMap[newButton] = _mouseMap[oldButton];
        _mouseMap.erase(oldButton);
    }

    void EventListener::addGamepadEvent(int gamepad, GamepadButton key, ButtonCallbacks func)
    {
        _gamepadMap[gamepad][key] = func;
    }

    void EventListener::removeGamepadEvent(int gamepad, GamepadButton key)
    {
        _gamepadMap[gamepad].erase(key);
    }

    void EventListener::replaceGamepadEvent(int gamepad, GamepadButton oldKey, GamepadButton newKey)
    {
        _gamepadMap[gamepad][newKey] = _gamepadMap[gamepad][oldKey];
        _gamepadMap[gamepad].erase(oldKey);
    }

    void EventListener::addGamepadStickEvent(int gamepad, int axis, std::function<void(SceneManager &, float)> func)
    {
        _gamepadStickMap[gamepad][axis] = func;
    }

    void EventListener::removeGamepadStickEvent(int gamepad, int axis)
    {
        _gamepadStickMap[gamepad].erase(axis);
    }

    void EventListener::removeAllGamepadEvents(int gamepad)
    {
        _gamepadMap.erase(gamepad);
    }

    std::map<KeyboardKey, ButtonCallbacks> &EventListener::getKeyboardMappings()
    {
        return _keyboardMap;
    }

    std::map<MouseButton, MouseCallbacks> &EventListener::getMouseMappings()
    {
        return _mouseMap;
    }

    std::map<GamepadButton, ButtonCallbacks> &EventListener::getGamepadMappings(int gamepad)
    {
        return _gamepadMap[gamepad];
    }

    std::map<int, std::function<void(SceneManager &, float)>> &EventListener::getGamepadStickMappings(int gamepad)
    {
        return _gamepadStickMap[gamepad];
    }

}
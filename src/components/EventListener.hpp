/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** EventListener
*/

#ifndef EVENTLISTENER_HPP_
#define EVENTLISTENER_HPP_

#include <functional>
#include <map>

#include "ButtonCallbacks.hpp"
#include "Component.hpp"
#include "GamepadStickCallbacks.hpp"
#include "MouseCallbacks.hpp"

namespace indie
{
    /// @brief EventListener is a class to describe what to do when an event is detected.
    /// you can add a callback to an event by using the different add<>Event methods.
    class EventListener : public Component
    {
    public:
        EventListener();
        ~EventListener();
        /**
         * @brief Adds a keyboard event to the listener
         *
         * @param key the key to listen to
         * @param callsbacks the callbacks to call when the key is interacted with
         */
        void addKeyboardEvent(KeyboardKey key, ButtonCallbacks callbacks);
        /**
         * @brief unbinds a key
         *
         * @param key The key to unbind
         */
        void removeKeyboardEvent(KeyboardKey key);
        /**
         * @brief updates the key for a listener
         *
         * @param oldKey the old key to take the callbacks from
         * @param newKey the new key that will receive the callbacks
         */
        void replaceKeyboardEvent(KeyboardKey oldKey, KeyboardKey newKey);
        /**
         * @brief Adds a mouse event to the listener
         *
         * @param key the key to listen to
         * @param func the function to call when the key is pressed
         */
        void addMouseEvent(MouseButton key, MouseCallbacks func);
        /**
         * @brief unbinds a mouseButton
         *
         * @param button The mouseButton to unbind
         */
        void removeMouseEvent(MouseButton button);
        /**
         * @brief replaces the key for a listener
         *
         * @param oldKey the old key to take the callbacks from
         * @param newKey the new key that will receive the callbacks
         */
        void replaceMouseEvent(MouseButton oldKey, MouseButton newKey);
        /**
         * @brief Adds a gamepad event to the listener
         *
         * @param gamepad the gamepad fow which the events are listened to
         * @param key the key to listen to
         * @param func the function to call when the key is pressed
         */
        void addGamepadEvent(int gamepad, GamepadButton key, ButtonCallbacks func);
        /**
         * @brief unbinds a GamepadButton
         *
         * @param button The GamepadButton to unbind
         */
        void removeGamepadEvent(int gamepad, GamepadButton key);
        /**
         * @brief replaces the key for a listener
         *
         * @param oldKey the old key to take the callbacks from
         * @param newKey the new key that will receive the callbacks
         */
        void replaceGamepadEvent(int gamepad, GamepadButton oldKey, GamepadButton newKey);
        /**
         * @brief Adds a gamepad stick event to the listener
         *
         * @param gamepad the gamepad fow which the events are listened to
         * @param axis the stick aswell as the axis to listen to
         * @param callbacks
         */
        void addGamepadStickEvent(int gamepad, int axis, std::function<void(SceneManager &, float)> callbacks);
        /**
         * @brief unbinds a GamepadStickAxis
         *
         * @param axis The GamepadStickAxis to unbind
         */
        void removeGamepadStickEvent(int gamepad, int axis);
        /**
         * @brief updates the key for a listener
         *
         * @param oldKey the old key to take the callbacks from
         * @param newKey the new key that will receive the callbacks
         */
        void removeAllGamepadEvents(int gamepad);
        /// @brief gets a reference to the keyboard mappings
        std::map<KeyboardKey, ButtonCallbacks> &getKeyboardMappings();
        /// @brief gets a reference to the mouse mappings
        std::map<MouseButton, MouseCallbacks> &getMouseMappings();
        /// @brief gets a reference to the gamepad mappings of a specified gamepad
        std::map<GamepadButton, ButtonCallbacks> &getGamepadMappings(int gamepad);
        /// @brief gets a reference to the gamepad stick mappings of a specified gamepad
        std::map<int, std::function<void(SceneManager &, float)>> &getGamepadStickMappings(int gamepad);

    protected:
    private:
        std::map<KeyboardKey, ButtonCallbacks> _keyboardMap;
        std::map<char, ButtonCallbacks> _keyboardCharMap;
        std::map<MouseButton, MouseCallbacks> _mouseMap;
        std::map<int, std::map<GamepadButton, ButtonCallbacks>> _gamepadMap;

        std::map<int, std::map<int, std::function<void(SceneManager &, float)>>> _gamepadStickMap;
    };
}

#endif /* !EVENTLISTENER_HPP_ */

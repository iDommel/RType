/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** GamepadStickCallbacks
*/

#ifndef GAMEPADCALLBACKS_HPP_
#define GAMEPADCALLBACKS_HPP_

#include <functional>

#include "SceneManager.hpp"

namespace indie
{
    /// @brief Class to group the gamepad events related to a gamepad's stick movements
    class GamepadStickCallbacks
    {
    public:
        GamepadStickCallbacks();
        /**
         * @brief Construct a new GamepadStickCallbacks object
         *
         * @param negative callback to be called when a gamepad is negative
         * @param null callback to be called when a gamepad is null
         * @param positive callack to be called when a gamepad is positive
         */
        GamepadStickCallbacks(
            std::function<void(SceneManager &, float)> negative,
            std::function<void(SceneManager &)> null,
            std::function<void(SceneManager &, float)> positive);
        ~GamepadStickCallbacks();
        std::function<void(SceneManager &, float)> _negative;
        std::function<void(SceneManager &)> _null;
        std::function<void(SceneManager &, float)> _positive;

    protected:
    private:
    };
} 

#endif /* !GAMEPADCALLBACKS_HPP_ */

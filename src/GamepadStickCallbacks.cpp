/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** GamepadStickCallbacks
*/

#include "GamepadStickCallbacks.hpp"

namespace ecs
{
    GamepadStickCallbacks::GamepadStickCallbacks()
    {
    }
    GamepadStickCallbacks::GamepadStickCallbacks(std::function<void(SceneManager &, float)> negative,
                                                 std::function<void(SceneManager &)> null,
                                                 std::function<void(SceneManager &, float)> positive) : _negative(negative),
                                                                                                        _null(null),
                                                                                                        _positive(positive)
    {
    }

    GamepadStickCallbacks::~GamepadStickCallbacks()
    {
    }
}
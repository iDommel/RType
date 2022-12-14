/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MouseCallbacks
*/

#include "MouseCallbacks.hpp"

namespace ecs
{

    MouseCallbacks::MouseCallbacks()
    {
    }
    MouseCallbacks::MouseCallbacks(std::function<void(SceneManager &, Vector2)> pressed,
                                   std::function<void(SceneManager &, Vector2)> released,
                                   std::function<void(SceneManager &, Vector2)> down,
                                   std::function<void(SceneManager &, Vector2)> up) : _pressed(pressed),
                                                                                      _released(released),
                                                                                      _down(down),
                                                                                      _up(up)
    {
    }

    MouseCallbacks::~MouseCallbacks()
    {
    }
}

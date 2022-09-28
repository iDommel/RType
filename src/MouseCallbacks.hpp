/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** MouseCallbacks
*/

#ifndef MOUSECALLBACKS_HPP_
#define MOUSECALLBACKS_HPP_

#include <functional>

#include "SceneManager.hpp"

struct Vector2;

namespace indie
{
    /// @brief Class to group all four callbacks for a mouse event
    class MouseCallbacks
    {
    public:
        MouseCallbacks();
        /**
         * @brief Construct a new MouseCallbacks object
         *
         * @param pressed callback to be called when a mousebutton is pressed
         * @param released callback to be called when a mousebutton is released
         * @param down callack to be called when a mousebutton is down
         * @param up callack to be called when a mousebutton is up
         */
        MouseCallbacks(
            std::function<void(SceneManager &, Vector2)> pressed,
            std::function<void(SceneManager &, Vector2)> released,
            std::function<void(SceneManager &, Vector2)> down,
            std::function<void(SceneManager &, Vector2)> up);
        ~MouseCallbacks();
        std::function<void(SceneManager &, Vector2)> _pressed;
        std::function<void(SceneManager &, Vector2)> _released;
        std::function<void(SceneManager &, Vector2)> _down;
        std::function<void(SceneManager &, Vector2)> _up;

    protected:
    private:
    };
} 

#endif /* !MOUSECALLBACKS_HPP_ */

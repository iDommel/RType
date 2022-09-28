/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** ButtonCallbacks
*/

#ifndef BUTTONCALLBACKS_HPP_
#define BUTTONCALLBACKS_HPP_

#include <functional>

#include "SceneManager.hpp"

namespace indie
{
    /// @brief Class to group all pressed, released and down callbacks for an event
    class ButtonCallbacks
    {
    public:
        ButtonCallbacks();
        /**
         * @brief Construct a new Button Callbacks object, every callback has a
         *  reference to the scenemenager and a pointer to the entity it should
         *  act on, if any.
         *
         * @param pressed callback to be called when a button is pressed
         * @param released callback to be called when a button is released
         * @param down callback to be called when a button is down
         */
        ButtonCallbacks(
            std::function<void(SceneManager &)> pressed,
            std::function<void(SceneManager &)> released,
            std::function<void(SceneManager &)> down,
            std::function<void(SceneManager &)> up);
        ~ButtonCallbacks();
        std::function<void(SceneManager &)> pressed;
        std::function<void(SceneManager &)> released;
        std::function<void(SceneManager &)> down;
        std::function<void(SceneManager &)> up;

    protected:
    private:
    };
} 

#endif /* !BUTTONCALLBACKS_HPP_ */

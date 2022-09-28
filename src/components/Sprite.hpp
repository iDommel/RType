/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** Sprite.hpp
*/

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <string>

#include "Component.hpp"

namespace indie
{

    class Sprite : public Component
    {
    public:
        /**
         * @brief Construct a new Sprite object
         * @param str Filepath to the texture
         * @param nbFrame number of frames in the picture, default is 0 (means no rect update)
         */
        Sprite(std::string str, int nbFrame = 0)
            : Component(Type::SPRITE), _value(str), _nbFrame(nbFrame)
        {
            _isInitialized = true;
        }

        /// @brief Returns component texture filepath
        std::string getValue() const { return _value; }
        /// @brief Returns component number of frames
        int getNbFrame() { return _nbFrame; }

    private:
        std::string _value;
        int _nbFrame;
    };

}

#endif /* !SPRITE_HPP */
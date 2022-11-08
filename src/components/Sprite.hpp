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

namespace ecs
{

    class Sprite : public Component
    {
    public:
        /**
         * @brief Construct a new Sprite object
         * @param str Filepath to the texture
         * @param rotation rotation in degrees
         * @param nbFrame number of frames in the picture, default is 0 (means no rect update)
         */
        Sprite(std::string str, float rotation = 0, float scale = 1.0, int nbFrame = 0)
            : Component(Type::SPRITE), _value(str), _nbFrame(nbFrame)
        {
            _isInitialized = true;
            _rotation = rotation;
            _scale = scale;
        }

        /// @brief Returns component texture filepath
        std::string getValue() const { return _value; }
        /// @brief Returns component number of frames
        int getNbFrame() { return _nbFrame; }
        /// @brief Returns component rotation value
        float getRotation() { return _rotation; }
        /// @brief Returns component scale value
        float getScale() { return _scale; }
        /// @brief Set component rotation value
        void setRotation(float rotation) { _rotation = rotation; }
        /// @brief Set component scale value
        void getRotation(float scale) { _scale = scale; }

    private:
        std::string _value;
        int _nbFrame;
        float _rotation;
        float _scale;
    };

}

#endif /* !SPRITE_HPP */
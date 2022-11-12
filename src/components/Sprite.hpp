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
        Sprite(std::string str, float rotation = 0, float scale = 1.0, Vector2 origin = {-1, -1})
            : Component(Type::SPRITE), _value(str), _origin(origin)
        {
            _isInitialized = true;
            _rotation = rotation;
            _scale = scale;
            _height = -1;
            _width = -1;
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
        /// @brief Returns the  origin value
        Vector2 getOrigin() { return _origin; }

        void setWidth(int width) { _width = width; }
        void setHeight(int height) { _height = height; }
        int getWidth() { return _width; }
        int getHeight() { return _height; }

    private:
        std::string _value;
        int _nbFrame;
        float _rotation;
        float _scale;
        float _width;
        float _height;
        Vector2 _origin;
    };

}

#endif /* !SPRITE_HPP */
/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** TextureError
*/

#ifndef TEXTUREERROR_HPP_
#define TEXTUREERROR_HPP_

#include "AssetError.hpp"

namespace ecs
{
    class TextureError : public AssetError
    {
    public:
        TextureError(const std::string &msg) : AssetError(msg) {}

    private:
        std::string _msg;
    };
};

#endif /* !TEXTUREERROR_HPP_ */

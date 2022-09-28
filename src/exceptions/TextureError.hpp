/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** TextureError
*/

#ifndef TEXTUREERROR_HPP_
#define TEXTUREERROR_HPP_

#include "AssetError.hpp"

namespace indie {
class TextureError: public AssetError {
    public:
        TextureError(const std::string &msg): AssetError(msg) {}
    private:
        std::string _msg;
};
};

#endif /* !TEXTUREERROR_HPP_ */

/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Texture2D
*/

#include "raylib.h"

#include "exceptions/TextureError.hpp"
#include "Texture2D.hpp"

namespace indie
{

    Texture::Texture(const std::string &fileName)
    {
        _texture = std::make_unique<Texture2D>(LoadTexture(fileName.c_str()));
        if (_texture->id == 0)
            throw TextureError("Texture constructor: Texture not loaded :" + fileName);
        _isLoaded = true;
    }

    Texture::~Texture()
    {
        if (_isLoaded)
            unload();
    }

    void Texture::load(const std::string &fileName)
    {
        if (_isLoaded)
            unload();
        _texture = std::make_unique<Texture2D>(LoadTexture(fileName.c_str()));
        if (_texture->id == 0)
            throw TextureError("Texture failed to load");
        _isLoaded = true;
    }

    void Texture::unload()
    {
        if (_isLoaded) {
            UnloadTexture(*_texture);
            _isLoaded = false;
        }
    }

    int Texture::getWidth() const
    {
        if (!_isLoaded)
            throw TextureError("Texture getWidth: Texture not loaded");
        return _texture->width;
    }

    int Texture::getHeight() const
    {
        if (!_isLoaded)
            throw TextureError("Texture getHeight: Texture not loaded");
        return _texture->height;
    }

    void Texture::draw(int posX, int posY, Color tint = WHITE)
    {
        if (!_isLoaded)
            throw TextureError("Texture draw: Texture not loaded");
        DrawTexture(*_texture, posX, posY, tint);
    }

    void Texture::draw(int posX, int posY)
    {
        if (!_isLoaded)
            throw TextureError("Texture draw: Texture not loaded");
        DrawTexture(*_texture, posX, posY, WHITE);
    }

    void Texture::setRect(float x, float y, float width, float height)
    {
        if (!_isLoaded)
            throw TextureError("Texture setRect: Texture not loaded");
        else if (!_rect)
            _rect = std::make_unique<Rectangle>();
        _rect->x = x;
        _rect->y = y;
        _rect->width = width;
        _rect->height = height;
    }

    void Texture::moveRect(int frame)
    {
        if (!_isLoaded)
            throw TextureError("Texture moveRect: Texture not loaded");
        else if (!_rect)
            throw TextureError("Texture moveRect: Rectangle not set");
        _rect->x = (float)frame * (float)getRect().width;
    }

    ::Rectangle Texture::getRect()
    {
        if (!_isLoaded)
            throw TextureError("Texture getRect: Texture not loaded");
        else if (!_rect)
            throw TextureError("Texture getRect: Rectangle not set");
        return *_rect;
    }

    void Texture::drawRec(Vector2 position, Color tint)
    {
        if (!_isLoaded)
            throw TextureError("Texture drawRec: Texture not loaded");
        else if (!_rect)
            throw TextureError("Texture drawRec: Rectangle not set");
        DrawTextureRec(*_texture, *_rect, position, tint);
    }

    void Texture::drawRec(Vector2 position)
    {
        if (!_isLoaded)
            throw TextureError("Texture drawRec: Texture not loaded");
        else if (!_rect)
            throw TextureError("Texture drawRec: Rectangle not set");
        DrawTextureRec(*_texture, *_rect, position, WHITE);
    }

    void Texture::operator=(const Texture2D &other)
    {
        if (other.id == 0)
            throw TextureError("Texture operator=: other Texture2D not loaded");
        if (_isLoaded)
            unload();
        _texture = std::make_unique<Texture2D>(other);
        if (_texture->id == 0)
            throw TextureError("Texture::operator=: Texture failed to load");
    }

}

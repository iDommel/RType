/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Text
*/

#include "raylib.h"

#include "Text.hpp"

namespace indie
{

    Text::Text(const std::string &text, const std::string &fileName)
    {
        if (fileName.empty())
            _font = std::make_unique<::Font>(GetFontDefault());
        else
            _font = std::make_unique<::Font>(LoadFont(fileName.c_str()));
        _text = text;
        _isLoaded = true;
    }

    Text::~Text()
    {
        if (_isLoaded)
            UnloadFont(*_font);
    }

    Font Text::getFontDefault(void) const
    {
        return GetFontDefault();
    }

    Font Text::getFont(void) const
    {
        return *_font;
    }

    void Text::loadFont(const std::string &fileName)
    {
        if (_isLoaded)
            UnloadFont(*_font);
        _font = std::make_unique<::Font>(LoadFont(fileName.c_str()));
        _isLoaded = true;
    }

    void Text::unloadFont(void)
    {
        if (_isLoaded)
            UnloadFont(*_font);
        _isLoaded = false;
    }

    void Text::draw(int posX, int posY, int fontSize, Color color)
    {
        DrawText(_text.c_str(), posX, posY, fontSize, color);
    }

    void Text::drawEx(Vector2 pos, float fontSize, float spacing, Color tint)
    {
        DrawTextEx(*_font, _text.c_str(), pos, fontSize, spacing, tint);
    }

    void Text::drawPro(Vector2 pos, Vector2 origin, float rotation, float fontSize, float spacing, Color tint)
    {
        DrawTextPro(*_font, _text.c_str(), pos, origin, rotation, fontSize, spacing, tint);
    }

    int Text::measure(int fontSize)
    {
        return MeasureText(_text.c_str(), fontSize);
    }

    Vector2 Text::measureEx(float fontSize, float spacing)
    {
        return MeasureTextEx(*_font, _text.c_str(), fontSize, spacing);
    }

}
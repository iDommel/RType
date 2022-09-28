/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Text
*/

#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <string>
#include <memory>

struct Vector2;
struct Color;
struct Font;

namespace indie
{
    class Text
    {
    public:
        Text(const std::string &text, const std::string &fileName = "");
        ~Text();

        /**
         * @brief Get the default Font
         * @return the default Font
         */
        Font getFontDefault(void) const;

        /**
         * @brief Get the loaded Font
         * @return The loaded Font
         */
        Font getFont(void) const;

        /**
         * @brief Load font from file into GPU memory (VRAM)
         * @param fileName Path to font file
         */
        void loadFont(const std::string &fileName);

        ///@brief Unload Font from GPU memory (VRAM)
        void unloadFont(void);

        /**
         * @brief Draw text (using default font)
         * @param posX Horizontal position of the text
         * @param posY Vertical position of the text
         * @param fontSize Font size of the text
         * @param color Color of the text
         */
        void draw(int posX, int posY, int fontSize = 10, Color color = WHITE);

        /**
         * @brief Draw text using font and additional parameters
         * @param pos Position of the text
         * @param fontSize Font size of the text
         * @param spacing Spacing between characters
         * @param color Color of the text
         */
        void drawEx(::Vector2 pos, float fontSize = 10, float spacing = 1, Color tint = WHITE);

        /**
         * @brief Draw text using Font and pro parameters (rotation)
         * @param pos Position of the text
         * @param origin Origin of the text
         * @param rotation Rotation of the text
         * @param fontSize Size of the text
         * @param spacing Spacing between letters
         * @param color Color of the text
         */
        void drawPro(Vector2 pos, Vector2 origin = {0, 0}, float rotation = 0.0f, float fontSize = 10, float spacing = 1, Color tint = WHITE);

        /**
         * @brief Measure string width for default font
         * @param text Text to measure
         * @param fontSize Size of the font
         * @return Size of the text
         */
        int measure(int fontSize = 10);

        /**
         * @brief Measure string size for Font
         * @param text Text to measure
         * @param fontSize Size of the font
         * @return Size of the text
         */
        Vector2 measureEx(float fontSize = 10, float spacing = 1);

    protected:
    private:
        std::string _text;
        std::unique_ptr<Font> _font;
        bool _isLoaded;
    };
}
#endif /* !TEXT_HPP_ */

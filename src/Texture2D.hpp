/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** Texture2D
*/

#ifndef TEXTURE2D_HPP_
#define TEXTURE2D_HPP_

#include <iostream>
#include <memory>

namespace sf {
    class Texture;
    class Sprite;
}

namespace ecs
{
    class Texture
    {
    public:
        Texture(const std::string &fileName);
        ~Texture();

        // /**
        //  * @brief Load texture from file into GPU memory (VRAM)
        //  * @param fileName The file name
        //  */
        // void load(const std::string &fileName);

        // /// @brief Unload texture from GPU memory (VRAM)
        // void unload();

        /// @brief Return the width of the loaded texture
        int getWidth() const;

        /// @brief Return the height of the loaded texture
        int getHeight() const;

        // /**
        //  * @brief Draw a Texture2D
        //  * @param posX The horizontal position
        //  * @param posY The vertical position
        //  * @param tint The color tint
        //  */
        // void draw(int posX, int posY, Color tint);

        /**
         * @brief Draw a Texture2D
         * @param posX The horizontal position
         * @param posY The vertical position
         */
        void draw(int posX, int posY);

        // /**
        //  * @brief Set the params of the texture's rect
        //  * @param x The horizontal position
        //  * @param y The vertical position
        //  * @param width The width of the rectangle
        //  * @param height The height of the rectangle
        //  */
        // void setRect(float x, float y, float width, float height);

        // /**
        //  * @brief Move the rect of the texture
        //  * @param currentFrame The number of frames to move the rectangle
        //  */
        // void moveRect(int frames);

        // ::Rectangle getRect();

        // /**
        //  * @brief Draw a part of a texture defined by a rectangle
        //  * @param source The rectangle defining the part of the texture to draw
        //  * @param position The position to draw the texture
        //  * @param tint The color tint
        //  */
        // void drawRec(Vector2 position, Color tint);

        // /**
        //  * @brief Draw a part of a texture defined by a rectangle
        //  * @param source The rectangle defining the part of the texture to draw
        //  * @param position The position to draw the texture
        //  */
        // void drawRec(Vector2 position);

        // /// @brief Copy a texture
        // void operator=(const Texture2D &other);

    protected:
    private:
        static std::map<std::string, std::shared_ptr<sf::Texture>> _textures;

        std::shared_ptr<sf::Texture> _texture = nullptr;
        std::unique_ptr<sf::Sprite> _sprite;
        // std::unique_ptr<Rectangle> _rect;
    };
}
#endif /* !TEXTURE2D_HPP_ */

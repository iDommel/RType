/*
** EPITECH PROJECT, 2022
** indieStudio [WSL : Ubuntu]
** File description:
** window
*/

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <string>

namespace sf
{
    class RenderWindow;
    class Color;
}  // namespace sf

namespace ecs
{
    class Window
    {
    public:
        /// @brief Construct a new window:: window object with the size of the screen
        Window(int screenWidth, int screenHeight, const std::string& name = "Indie Studio");
        /// @brief Destruct the window:: close the windows
        ~Window();
        /**
         * @brief asks if the window has received a closing event
         *
         * @return true if it should close, false if it shouldn't
         */
        bool shouldClose();

        /// @brief Clears the window, black by default
        void clear(const sf::Color& color = sf::Color(0, 0, 0, 255));

        /// @brief Display the current contents of the window
        void display();

        int getScreenWidth();
        /// @brief Get the screen height
        int getScreenHeight();

    private:
        sf::RenderWindow _win;
        static bool _shouldClose;
    };
}
#endif /* !WINDOW_HPP_ */
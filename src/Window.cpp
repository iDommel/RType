/*
** EPITECH PROJECT, 2022
** indieStudio [WSL : Ubuntu]
** File description:
** window
*/

#include <SFML/Graphics.hpp>

#include "Window.hpp"

namespace ecs
{
    bool Window::_shouldClose = false;

    Window::Window(int screenWidth, int screenHeight, const std::string& name)
    {
        _win = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), name);
    }

    Window::~Window()
    {
        _win->close();
    }

    bool Window::shouldClose()
    {
        return _shouldClose;
    }

    void Window::clear(const sf::Color& color)
    {
        _win->clear(color);
    }

    void Window::display()
    {
        _win->display();
    }

    int Window::getScreenWidth()
    {
        return (_win->getSize().x);
    }

    int Window::getScreenHeight()
    {
        return (_win->getSize().y);
    }
}
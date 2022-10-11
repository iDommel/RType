/*
** EPITECH PROJECT, 2022
** indieStudio [WSL : Ubuntu]
** File description:
** window
*/

#include "raylib.h"

#include "Window.hpp"

namespace indie
{

    Window::Window(int screenWidth, int screenHeight, const std::string& name)
    {
        InitWindow(screenWidth, screenHeight, name.c_str());
    }

    Window::Window(int screenWidth, int screenHeight, unsigned int flags, const std::string& name)
    {
        SetConfigFlags(flags);
        InitWindow(screenWidth, screenHeight, name.c_str());
    }

    Window::~Window()
    {
        CloseWindow();
    }

    int Window::getKeyPressed()
    {
        int key = GetKeyPressed();
        return (key);
    }

    bool Window::shouldClose()
    {
        return (WindowShouldClose());
    }

    void Window::beginDraw()
    {
        BeginDrawing();
    }

    void Window::clearBackground(Color color)
    {
        ClearBackground(color);
    }

    void Window::endDraw()
    {
        EndDrawing();
    }

    Vector2 Window::getMousePosition()
    {
        Vector2 mp = {0, 0};

        mp = GetMousePosition();
        return (mp);
    }

    void Window::showCursor()
    {
        ShowCursor();
    }

    void Window::hideCursor()
    {
        HideCursor();
    }

    void Window::beginMode3D(Camera3D camera)
    {
        BeginMode3D(camera);
    }

    void Window::beginMode2D(Camera2D camera)
    {
        BeginMode2D(camera);
    }

    void Window::endMode3D()
    {
        EndMode3D();
    }

    void Window::endMode2D()
    {
        EndMode2D();
    }

    bool Window::isMouseButtonPressed(int button)
    {
        return IsMouseButtonPressed(button);
    }

    bool Window::isMouseButtonDown(int button)
    {
        return IsMouseButtonDown(button);
    }

    bool Window::isMouseButtonReleased(int button)
    {
        return IsMouseButtonReleased(button);
    }

    bool Window::isMouseButtonUp(int button)
    {
        return IsMouseButtonUp(button);
    }

    bool Window::isGamepadAvailable(int gamepad)
    {
        return IsGamepadAvailable(gamepad);
    }

    const char* Window::getGamepadName(int gamepad)
    {
        return GetGamepadName(gamepad);
    }

    bool Window::isGamepadButtonPressed(int gamepad, int button)
    {
        return IsGamepadButtonPressed(gamepad, button);
    }

    bool Window::isGamepadButtonDown(int gamepad, int button)
    {
        return IsGamepadButtonDown(gamepad, button);
    }

    bool Window::isGamepadButtonReleased(int gamepad, int button)
    {
        return IsGamepadButtonReleased(gamepad, button);
    }

    bool Window::isGamepadButtonUp(int gamepad, int button)
    {
        return IsGamepadButtonUp(gamepad, button);
    }

    int Window::getGamepadButtonPressed(void)
    {
        return GetGamepadButtonPressed();
    }

    int Window::getGamepadAxisCount(int gamepad)
    {
        return GetGamepadAxisCount(gamepad);
    }

    float Window::getGamepadAxisMovement(int gamepad, int axis)
    {
        return GetGamepadAxisMovement(gamepad, axis);
    }

    int Window::setGamepadMappings(const char* mappings)
    {
        return SetGamepadMappings(mappings);
    }

    bool Window::isKeyPressed(int key)
    {
        return IsKeyPressed(key);
    }
    bool Window::isKeyDown(int key)
    {
        return IsKeyDown(key);
    }
    bool Window::isKeyReleased(int key)
    {
        return IsKeyReleased(key);
    }
    bool Window::isKeyUp(int key)
    {
        return IsKeyUp(key);
    }
    void Window::setExitKey(int key)
    {
        SetExitKey(key);
    }

    int Window::getScreenWidth()
    {
        int position = GetScreenWidth();

        return (position);
    }

    int Window::getScreenHeight()
    {
        int position = GetScreenHeight();

        return (position);
    }
}
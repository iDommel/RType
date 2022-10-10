/*
** EPITECH PROJECT, 2022
** indieStudio [WSL : Ubuntu]
** File description:
** window
*/

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <iostream>
#include <string>
#include <vector>

struct Color;
struct Camera2D;
struct Camera3D;
struct Vector2;
namespace indie
{
    class Window
    {
    public:
        /// @brief Construct a new window:: window object with the size of the screen
        Window(int screenWidth, int screenHeight, const std::string& name = "Indie Studio");
        /// @brief Construct a new window:: window object with the size of the screen, pass configuration flags as parameter
        Window(int screenWidth, int screenHeight, unsigned int flags, const std::string& name = "Indie Studio");
        /// @brief Destruct the window:: close the windows
        ~Window();
        /**
         * @brief asks if the window has received a closing event
         *
         * @return true if it should close, false if it shouldn't
         */
        static bool shouldClose();
        /// @brief function that get the key pressed
        static int getKeyPressed();
        /// @brief Start window drawing state
        static void beginDraw();
        /// @brief funcion that clear the background
        static void clearBackground(Color color);
        /// @brief final function of the loop, end the drawing
        static void endDraw();
        /// @brief get the mouse position
        static Vector2 getMousePosition();
        /// @brief Shows Cursor
        static void showCursor();
        ///  @brief Hide Cursor
        static void hideCursor();
        /**
         * @brief Begin 2D mode with custom camera
         * @param camera the camera to be used
         */
        static void beginMode2D(Camera2D camera);
        /**
         * @brief Begin 3D mode with custom camera
         * @param camera the camera to be used
         */
        static void beginMode3D(Camera3D camera);
        /// @brief End 3D mode.
        static void endMode3D();
        /// @brief End 2D mode.
        static void endMode2D();
        /// @brief Check if a mouse button has been pressed once
        static bool isMouseButtonPressed(int button);
        /// @brief Check if a mouse button is being pressed
        static bool isMouseButtonDown(int button);
        /// @brief Check if a mouse button has been released once
        static bool isMouseButtonReleased(int button);
        /// @brief Check if a mouse button is NOT being pressed
        static bool isMouseButtonUp(int button);
        /// @brief Check if a gamepad is available
        static bool isGamepadAvailable(int gamepad);
        /// @brief  Get gamepad internal name id
        static const char* getGamepadName(int gamepad);
        /// @brief  Check if a gamepad button has been pressed once
        static bool isGamepadButtonPressed(int gamepad, int button);
        /// @brief  Check if a gamepad button is being pressed
        static bool isGamepadButtonDown(int gamepad, int button);
        /// @brief  Check if a gamepad button has been released once
        static bool isGamepadButtonReleased(int gamepad, int button);
        /// @brief  Check if a gamepad button is NOT being pressed
        static bool isGamepadButtonUp(int gamepad, int button);
        /// @brief  Get the last gamepad button pressed
        static int getGamepadButtonPressed(void);
        /// @brief  Get gamepad axis count for a gamepad
        static int getGamepadAxisCount(int gamepad);
        /// @brief  Get axis movement value for a gamepad axis
        static float getGamepadAxisMovement(int gamepad, int axis);
        /// @brief Set internal gamepad mappings (SDL_GameControllerDB)
        static int setGamepadMappings(const char* mappings);
        /// @brief Check if a key has been pressed once
        static bool isKeyPressed(int key);
        /// @brief Check if a key is being pressed
        static bool isKeyDown(int key);
        /// @brief Check if a key has been released once
        static bool isKeyReleased(int key);
        /// @brief Check if a key is NOT being pressed
        static bool isKeyUp(int key);
        ///  @brief Set a custom key to exit program(default is ESC)
        static void setExitKey(int key);
        /// @brief Get the screen width
        static int getScreenWidth();
        /// @brief Get the screen height
        static int getScreenHeight();

    private:
    };
}
#endif /* !WINDOW_HPP_ */
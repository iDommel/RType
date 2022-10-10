/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** test_textures
*/

#include "raylib.h"

#include "Texture2D.hpp"

void testTextures()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - texture loading and drawing");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    indie::Texture texture("test_pictures/raylib_logo.png");  // Texture loading
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        texture.draw(screenWidth / 2 - texture.getWidth() / 2, screenHeight / 2 - texture.getHeight() / 2, WHITE);

        DrawText("this IS a texture!", 360, 370, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    texture.unload();  // Texture unloading
    //--------------------------------------------------------------------------------------

    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
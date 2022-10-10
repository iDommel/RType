/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** TestText
*/

#include "raylib.h"

#include "Text.hpp"

void testText(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [text] example - font loading");

    // Define characters to draw
    // NOTE: raylib supports UTF-8 encoding, following list is actually codified as UTF8 internally
    const char msg[256] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    // BMFont (AngelCode) : Font data and image atlas have been generated using external program
    indie::Text textMsg(msg, "test_fonts/pixantiqua.fnt");
    indie::Text textBm("Using BMFont (Angelcode) imported");
    indie::Text textSpace("Hold SPACE to use TTF generated font");

    // TTF font : Font data and atlas are generated directly from TTF
    // NOTE: We define a font base size of 32 pixels tall and up-to 250 characters

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        textSpace.draw(20, 20, 20, LIGHTGRAY);

        textMsg.drawEx((Vector2){20.0f, 100.0f}, (float)textMsg.getFont().baseSize, 2, MAROON);
        textBm.draw(20, GetScreenHeight() - 30, 20, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    textMsg.unloadFont();  // AngelCode Font unloading

    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** TestAnimations2D
*/

#include "raylib.h"

#include "Texture2D.hpp"

#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED 1

void testAnimations2D()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - texture rectangle");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    indie::Texture scarfy("test_pictures/scarfy.png");  // Texture loading

    scarfy.setRect(0.0f, 0.0f, (float)scarfy.getWidth() / 6, (float)scarfy.getHeight());

    Vector2 position = {350.0f, 280.0f};
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 8;  // Number of spritesheet frames shown by second

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        framesCounter++;

        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 5)
                currentFrame = 0;

            scarfy.moveRect(currentFrame);
        }

        if (IsKeyPressed(KEY_RIGHT))
            framesSpeed++;
        else if (IsKeyPressed(KEY_LEFT))
            framesSpeed--;

        if (framesSpeed > MAX_FRAME_SPEED)
            framesSpeed = MAX_FRAME_SPEED;
        else if (framesSpeed < MIN_FRAME_SPEED)
            framesSpeed = MIN_FRAME_SPEED;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        scarfy.draw(15, 40, WHITE);
        DrawRectangleLines(15, 40, scarfy.getWidth(), scarfy.getHeight(), LIME);
        DrawRectangleLines(15 + (int)scarfy.getRect().x, 40 + (int)scarfy.getRect().y, (int)scarfy.getRect().width, (int)scarfy.getRect().height, RED);

        DrawText("FRAME SPEED: ", 165, 210, 10, DARKGRAY);
        DrawText(TextFormat("%02i FPS", framesSpeed), 575, 210, 10, DARKGRAY);
        DrawText("PRESS RIGHT/LEFT KEYS to CHANGE SPEED!", 290, 240, 10, DARKGRAY);

        for (int i = 0; i < MAX_FRAME_SPEED; i++) {
            if (i < framesSpeed)
                DrawRectangle(250 + 21 * i, 205, 20, 20, RED);
            DrawRectangleLines(250 + 21 * i, 205, 20, 20, MAROON);
        }

        scarfy.drawRec(position, WHITE);  // Draw part of the texture

        DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    scarfy.unload();  // Texture unloading

    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
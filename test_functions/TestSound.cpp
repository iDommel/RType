/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** TestSound.cpp
*/

#include "Sound.hpp"
#include "raylib.h"

void testSound(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [audio] example - sound loading and playing");
    InitAudioDevice();  // Initialize audio device
    indie::Sound s1("test_pictures/jump.wav");
    Sound fxWav = LoadSound("test_pictures/jump.wav");  // Load WAV audio file

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_SPACE))
            s1.play();  // Play WAV sound
        if (IsKeyPressed(KEY_ENTER))
            PlaySound(fxWav);  // Play OGG sound
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Press SPACE to PLAY the WAV sound!", 200, 180, 20, LIGHTGRAY);
        DrawText("Press ENTER to PLAY the OGG sound!", 200, 220, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    s1.~Sound();         // Unload sound data
    UnloadSound(fxWav);  // Unload sound data
    CloseAudioDevice();  // Close audio device

    CloseWindow();  // Close window and OpenGL context
    //--
}
/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** TestMusic
*/

#include "raylib.h"
#include <stdlib.h>

#include "../src/Music.hpp"

void testMusic(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [audio] example - music playing (streaming)");

    InitAudioDevice();

    indie::Music music("test_pictures/music.ogg");

    music.playMusic();

    float timePlayed = 0.0f;
    bool pause = false;


    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        music.updateMusic();

        if (IsKeyPressed(KEY_SPACE))
        {
            music.stopMusic();
            music.playMusic();
        }

        if (IsKeyPressed(KEY_P))
        {
            pause = !pause;

            if (pause) music.pauseMusic();
            else music.resumeMusic();
        }

        timePlayed = music.getTimePlayedMusic()/music.getTimeLengthMusic()*400;

        if (timePlayed > 400) music.stopMusic();


        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("MUSIC SHOULD BE PLAYING!", 255, 150, 20, LIGHTGRAY);

            DrawRectangle(200, 200, 400, 12, LIGHTGRAY);
            DrawRectangle(200, 200, (int)timePlayed, 12, MAROON);
            DrawRectangleLines(200, 200, 400, 12, GRAY);

            DrawText("PRESS SPACE TO RESTART MUSIC", 215, 250, 20, LIGHTGRAY);
            DrawText("PRESS P TO PAUSE/RESUME MUSIC", 208, 280, 20, LIGHTGRAY);

        EndDrawing();
    }

    music.~Music();

    CloseAudioDevice();

    CloseWindow();
}
/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** TestCamera
*/
#include <exception>
#include <iostream>
#include "raylib.h"

#include "raylib.h"

#include "Camera.hpp"
#include "EventListener.hpp"
#include "Shape3D.hpp"
#include "Window.hpp"

void testRaylibCamera()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    Vector3 cameraPosition = {0.0f, 10.0f, 10.0f};
    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

    InitWindow(screenWidth, screenHeight, "3d camera encapsulation test");

    std::shared_ptr<Vector3> cubePositionPtr = std::make_shared<Vector3>(cubePosition);
    std::shared_ptr<Vector3> cameraPositionPtr = std::make_shared<Vector3>(cameraPosition);
    indie::Camera camera(cubePosition, cameraPosition);

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        if (IsKeyDown(KEY_RIGHT)) {
            cubePosition.x -= 0.1f;
        } else if (IsKeyDown(KEY_LEFT)) {
            cubePosition.x += 0.1f;
        }
        if (IsKeyDown(KEY_UP)) {
            cubePosition.y += 0.1f;
        } else if (IsKeyDown(KEY_DOWN)) {
            cubePosition.y -= 0.1f;
        }
        if (IsKeyDown(KEY_RIGHT_SHIFT)) {
            cubePosition.z += 0.1f;
        } else if (IsKeyDown(KEY_RIGHT_CONTROL)) {
            cubePosition.z -= 0.1f;
        }
        if (IsKeyDown(KEY_KP_ADD)) {
            cameraPosition.y += 0.1f;
        } else if (IsKeyDown(KEY_KP_SUBTRACT)) {
            cameraPosition.y -= 0.1f;
        }
        camera.update();  // Update camera
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        camera.beginDrawScope();

        DrawCube(*cubePositionPtr, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(*cubePositionPtr, 2.0f, 2.0f, 2.0f, MAROON);

        DrawGrid(10, 1.0f);

        camera.endDrawScope();

        DrawRectangle(10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(10, 10, 320, 133, BLUE);

        DrawText("- Use arrows, right-ctrl and right-shift to move the object", 40, 40, 10, DARKGRAY);
        DrawText("- Use + and - to move the cam", 40, 60, 10, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
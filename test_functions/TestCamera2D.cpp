/*
** EPITECH PROJECT, 2022
** RType
** File description:
** TestCamera
*/

#include <exception>
#include <iostream>
#include <raylib.h>
#include "Camera2D.hpp"
#include "components/EventListener.hpp"
#include "Shape2D.hpp"
#include "Window.hpp"

void testRaylibCamera2D()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    Vector2 cameraPosition = {0.0f, 10.0f};
    Vector2 rectPosition = {0.0f, 0.0f};

    InitWindow(screenWidth, screenHeight, "2D camera encapsulation test");

    std::shared_ptr<Vector2> rectPositionPtr = std::make_shared<Vector2>(rectPosition);
    std::shared_ptr<Vector2> cameraPositionPtr = std::make_shared<Vector2>(cameraPosition);
    rtype::Camera2D camera(rectPositionPtr);

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        if (IsKeyDown(KEY_RIGHT)) {
            rectPositionPtr->x -= 1.0f;
        } else if (IsKeyDown(KEY_LEFT)) {
            rectPositionPtr->x += 1.0f;
        }
        if (IsKeyDown(KEY_UP)) {
            rectPositionPtr->y += 1.0f;
        } else if (IsKeyDown(KEY_DOWN)) {
            rectPositionPtr->y -= 1.0f;
        }
        if (IsKeyDown(KEY_KP_ADD)) {
            cameraPosition.y += 1.0f;
        } else if (IsKeyDown(KEY_KP_SUBTRACT)) {
            cameraPosition.y -= 1.0f;
        }
        camera.update();  // Update camera
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        camera.beginDrawScope();

        DrawRectangle(rectPositionPtr->x, rectPositionPtr->y, 200, 200, RED);
        DrawRectangleLines(0, 0, 200, 200, BLUE);

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
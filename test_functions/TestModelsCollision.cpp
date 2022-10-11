/*
** EPITECH PROJECT, 2022
** indieStudio
** File description:
** TestModelsCollision
*/

#include "CollideSystem.hpp"
#include "../src/Model.hpp"

#include "raylib.h"

void testModelsCollision(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    indie::CollideSystem collideSystem;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - models loading");


    // Define the camera to look into our 3d world
    Vector3 a = { 0.0f, 10.0f, 10.0f };
    Vector3 b = { 0.0f, 0.0f, 0.0f };
    Vector3 c = { 0.0f, 1.0f, 0.0f };
    Camera camera = { a, b, c, 45.0f, 0 };

    indie::Model test("test_models/turret.obj", "test_models/turret.png");
    indie::Model test2("test_models/turret.obj", "test_models/turret.png");

    Vector3 position = { 0.0f, 0.0f, 0.0f };                    // Set model position
    Vector3 position2 = { 10.0f, 0.0f, 10.0f };                 // Set model2 position

    Vector3 rotation = { 10.0f, 0.0f, 0.0f };
    Vector3 rotation2 = { 0.0f, 0.0f, 0.0f };
    Vector3 scale = { 1.0f, 1.0f, 1.0f };
    SetCameraMode(camera, CAMERA_FREE);     // Set a free camera mode

    bool colliding = false;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) {
            position2.x += 0.2f;
        } else if (IsKeyDown(KEY_LEFT)) {
            position2.x -= 0.2f;
        }
        if (IsKeyDown(KEY_DOWN)) {
            position2.z += 0.2f;
        } else if (IsKeyDown(KEY_UP)) {
            position2.z -= 0.2f;
        }
        colliding = false;

        if (collideSystem.check3DCollision(
            collideSystem.makeUpdatedBBox(test.getBoundingBox(), position),
            collideSystem.makeUpdatedBBox(test2.getBoundingBox(), position2)))
            colliding = true;

        UpdateCamera(&camera);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                test.drawRotate(position, rotation, 45.0f, scale, WHITE);
                test2.drawRotate(position2, rotation2, 45.0f, scale, WHITE);
                DrawBoundingBox(collideSystem.makeUpdatedBBox(test.getBoundingBox(), position), GREEN);
                DrawBoundingBox(collideSystem.makeUpdatedBBox
                (test2.getBoundingBox(), position2), GREEN);

                DrawGrid(20, 10.0f);         // Draw a grid

            EndMode3D();

            if (colliding) DrawText("MODELS COLLIDING", GetScreenWidth() - 110, 10, 10, GREEN);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    test.unload();
    CloseWindow();              // Close window and OpenGL context
    //--------------------
}
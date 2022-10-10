#include "raylib.h"
#include "../src/ModelAnimation.hpp"
#include "../src/Model.hpp"

void test_raylib_modelAnimation()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - model animation");

    // Define the camera to look into our 3d world
    Vector3 a = { 0.0f, 10.0f, 10.0f };
    Vector3 b = { 0.0f, 0.0f, 0.0f };
    Vector3 c = { 0.0f, 1.0f, 0.0f };
    Camera camera = { a, b, c, 45.0f, 0 };

    std::unique_ptr<indie::Model> model = std::make_unique<indie::Model>("assets_test/final.iqm", "assets_test/playerobj.png");
    Vector3 position = { 0.0f, 0.0f, 0.0f };

    indie::ModelAnimation anim("assets_test/final_anim.iqm");
    int animFrameCounter = 0;

    SetCameraMode(camera, CAMERA_FREE);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera);

        if (IsKeyDown(KEY_SPACE))
        {
            animFrameCounter++;
            anim.updateModelAnimation(*model, animFrameCounter);
            if (animFrameCounter >= anim.getFrameCount()) animFrameCounter = 0;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                Vector3 x = { 1.0f, 0.0f, 0.0f };
                Vector3 x2 = { 1.0f, 1.0f, 1.0f };
                DrawModelEx(model->getModel(), position, x, -90.0f, x2, WHITE);

        DrawGrid(10, 1.0f);

        EndMode3D();

        EndDrawing();
    }

    anim.unloadModelAnimation();

    CloseWindow();
}
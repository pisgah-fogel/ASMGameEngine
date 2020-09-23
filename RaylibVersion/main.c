#define RMEM_IMPLEMENTATION // ObjPool
#define SUPPORT_TRACELOG
#define SUPPORT_TRACELOG_DEBUG
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"

#include "Nodes/node.h"
#include "Nodes/sprite.h"
#include "Nodes/texture.h"
#include "Nodes/label.h"

// #define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

/**
 * @brief Render everything
 */
void UpdateDrawFrame();

// TODO: implement other type of camera movements
void CameraSmoothFollow(Camera2D *camera, Vector2 target, float delta);

node_base_t *sprite_test;
node_base_t *texture_test;
node_base_t *label_test;

Camera2D camera = { 0 };

int main()
{
    node_root_setup();

    sprite_test = create_sprite();
    node_root_set_head(sprite_test);

    texture_test = create_texture();
    node_add_child(sprite_test, texture_test);

    label_test = create_label();
    node_add_child(sprite_test, label_test);

    list_print(sprite_test->childs);

    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){ (float)node_get_root()->screenWidth/2, (float)node_get_root()->screenWidth/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1); // Web platforms does not like never endind loops
#else
    SetTargetFPS(60);
    // Main Game Loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    node_root_free();
    printf("No segfault\n");
    return 0;
}

Vector2 cursorPosition = { -100.0f, -100.0f };
Color cursorColor = DARKBLUE;

void UpdateDrawFrame()
{
    float delta = GetFrameTime();
    // Handle Events
    node_root_event();
    cursorPosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) cursorColor = MAROON;
    else if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) cursorColor = LIME;
    else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) cursorColor = DARKBLUE;

    node_root_process();

    // TODO: run physic engine

    CameraSmoothFollow(&camera, (Vector2){((sprite_t*)sprite_test->data)->dest.x, ((sprite_t*)sprite_test->data)->dest.y}, delta);

    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        node_root_render();

        EndMode2D();

        DrawCircleV(cursorPosition, 40, cursorColor);

        DrawText("Everythings' working fine so far...", 190, 200, 20, LIGHTGRAY);

        DrawText(TextFormat("DETECTED AXIS [%i]:", GetGamepadAxisCount(GAMEPAD_PLAYER1)), 10, 50, 10, MAROON);

        for (int i = 0; i < GetGamepadAxisCount(GAMEPAD_PLAYER1); i++)
        {
            DrawText(TextFormat("AXIS %i: %.02f", i, GetGamepadAxisMovement(GAMEPAD_PLAYER1, i)), 20, 70 + 20*i, 10, DARKGRAY);
        }

        if (GetGamepadButtonPressed() != -1) DrawText(TextFormat("DETECTED BUTTON: %i", GetGamepadButtonPressed()), 10, 430, 10, RED);
        else DrawText("DETECTED BUTTON: NONE", 10, 430, 10, GRAY);

    EndDrawing();
}

void CameraSmoothFollow(Camera2D *camera, Vector2 target, float delta)
{
    const float minSpeed = 30;
    const float minEffectLength = 10;
    const float fractionSpeed = 0.8f;
    
    Vector2 diff = Vector2Subtract(target, camera->target);
    float length = Vector2Length(diff);
    
    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed*delta/length));
    }
}
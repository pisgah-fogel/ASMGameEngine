#define RMEM_IMPLEMENTATION // ObjPool
#define SUPPORT_TRACELOG
#define SUPPORT_TRACELOG_DEBUG
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "Nodes/node.h"
#include "Nodes/sprite.h"
#include "Nodes/texture.h"

// #define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

/**
 * @brief Render everything
 */
void UpdateDrawFrame();

node_base_t *sprite_test;

node_base_t *texture_test;

node_root_t* root;

int main()
{
    root = node_root_init(450, 800, "Node Test");

    const char* filename = "save.txt";
    if (FileExists(filename)) {
        TraceLog(LOG_INFO, "File %s exist", filename);
        
        char * buff = LoadFileText(filename);
        if (buff == NULL)
            TraceLog(LOG_INFO, "LoadFileText failed");
        else
            TraceLog(LOG_INFO, "LoadFileText read %s", buff);
    } else {
        TraceLog(LOG_INFO, "File %s does not exist, creating it...", filename);
        SaveFileText(filename, "42");
    }

    sprite_test = create_sprite();
    node_init(sprite_test);

    texture_test = create_texture();
    node_init(texture_test);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1); // Web platform does not like never endind loops
#else
    SetTargetFPS(60);
    // Main Game Loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    node_free(sprite_test);
    node_free(texture_test);
    node_root_free(&root);
    printf("No segfault\n");
    return 0;
}

Vector2 cursorPosition = { -100.0f, -100.0f };
Color cursorColor = DARKBLUE;

void UpdateDrawFrame()
{
    // Handle Events
    node_event(sprite_test);
    cursorPosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) cursorColor = MAROON;
    else if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) cursorColor = LIME;
    else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) cursorColor = DARKBLUE;

    // TODO: call _process on each node
    node_process(sprite_test);

    // TODO: run physic engine

    BeginDrawing();

        ClearBackground(RAYWHITE);

        node_render(texture_test);

        node_render(sprite_test);
        
        DrawText("Everythings' working fine so far...", 190, 200, 20, LIGHTGRAY);

        DrawText(TextFormat("DETECTED AXIS [%i]:", GetGamepadAxisCount(GAMEPAD_PLAYER1)), 10, 50, 10, MAROON);

        for (int i = 0; i < GetGamepadAxisCount(GAMEPAD_PLAYER1); i++)
        {
            DrawText(TextFormat("AXIS %i: %.02f", i, GetGamepadAxisMovement(GAMEPAD_PLAYER1, i)), 20, 70 + 20*i, 10, DARKGRAY);
        }

        if (GetGamepadButtonPressed() != -1) DrawText(TextFormat("DETECTED BUTTON: %i", GetGamepadButtonPressed()), 10, 430, 10, RED);
        else DrawText("DETECTED BUTTON: NONE", 10, 430, 10, GRAY);

        DrawCircleV(cursorPosition, 40, cursorColor);

    EndDrawing();
}

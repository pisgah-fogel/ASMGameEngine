#define RMEM_IMPLEMENTATION // ObjPool
#define SUPPORT_TRACELOG
#define SUPPORT_TRACELOG_DEBUG
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "rmem.h"

// #define PLATFORM_WEB
#define MAX_SPRITE 10

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

int screenWidth = 800;
int screenHeight = 450;

/**
 * @brief Render everything
 */
void UpdateDrawFrame();

/**
 * @brief Sprites can be rotated and scaled
 */
typedef struct sprite {
    Texture2D texture;
    Rectangle clip;
    Rectangle dest;
    Vector2 center; // 0, 0 is the top left of the clipped texture
    int rotation;
    Color tint;
} sprite_t;

ObjPool sprite_pool;
sprite_t *sprite_test;

/**
 * @brief Same as sprites but does not rotate or get scaled
 */
typedef struct texture {
    Texture2D texture;
    int x;
    int y;
    Color tint;
} texture_t;

texture_t *texture_test;

int main()
{
    InitWindow(screenWidth, screenHeight, "POC raylib");
    
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

    sprite_pool = CreateObjPool(sizeof(sprite_t), MAX_SPRITE);

    sprite_test = ObjPoolAlloc(&sprite_pool);
    sprite_test->texture = LoadTexture("resources/spritesheet.png");
    sprite_test->clip = (Rectangle){ 100.0f, 0.0f, 100.0f, 100.0f };
    sprite_test->dest = (Rectangle){ screenWidth/2, screenHeight/2, 100.0f, 100.0f };
    sprite_test->center = (Vector2){ 50.0f, 50.0f };
    sprite_test->rotation = 0;
    sprite_test->tint = WHITE;

    texture_test = (texture_t*)malloc(sizeof(texture_t));
    texture_test->texture = LoadTexture("resources/spritesheet.png");
    texture_test->x = 0;
    texture_test->y = 0;
    texture_test->tint = (Color){0, 200, 200, 150};

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

    // TODO: Free resources
    UnloadTexture(sprite_test->texture);
    ObjPoolCleanUp(&sprite_pool, (void **)&sprite_test);
    DestroyObjPool(&sprite_pool);

    UnloadTexture(texture_test->texture);
    free(texture_test);

    CloseWindow();
    return 0;
}

Vector2 cursorPosition = { -100.0f, -100.0f };
Color cursorColor = DARKBLUE;

void UpdateDrawFrame()
{
    // Handle Events
    if (IsKeyDown(KEY_RIGHT)) sprite_test->dest.x += 2.0f;
    if (IsKeyDown(KEY_LEFT)) sprite_test->dest.x -= 2.0f;
    if (IsKeyDown(KEY_UP)) sprite_test->dest.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN)) sprite_test->dest.y += 2.0f;
    cursorPosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) cursorColor = MAROON;
    else if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) cursorColor = LIME;
    else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) cursorColor = DARKBLUE;

    // TODO: call _process on each node
    sprite_test->rotation++;

    // TODO: run physic engine

    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(texture_test->texture, texture_test->x, texture_test->y, texture_test->tint);

        DrawTexturePro(sprite_test->texture, sprite_test->clip, sprite_test->dest, sprite_test->center, (float)sprite_test->rotation, sprite_test->tint);
        DrawLine((int)sprite_test->dest.x, 0, (int)sprite_test->dest.x, screenHeight, GRAY);
        DrawLine(0, (int)sprite_test->dest.y, screenWidth, (int)sprite_test->dest.y, GRAY);
        
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

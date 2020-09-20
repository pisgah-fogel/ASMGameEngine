#define RMEM_IMPLEMENTATION // ObjPool
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
 * 
 */
void UpdateDrawFrame();

typedef struct sprite {
    Texture2D texture;
    Rectangle clip;
    Rectangle dest;
    Vector2 center;
    int rotation;
} sprite_t;

ObjPool sprite_pool;
sprite_t *texture_test;

int main()
{
    InitWindow(screenWidth, screenHeight, "POC raylib");

    sprite_pool = CreateObjPool(sizeof(sprite_t), MAX_SPRITE);

    texture_test = ObjPoolAlloc(&sprite_pool);
    texture_test->texture = LoadTexture("resources/spritesheet.png");
    texture_test->clip = (Rectangle){ 0.0f, 0.0f, 100.0f, 100.0f };
    texture_test->dest = (Rectangle){ screenWidth/2, screenHeight/2, 100.0f, 100.0f };
    texture_test->center = (Vector2){ 50.0f, 50.0f };
    texture_test->rotation = 0;

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
    UnloadTexture(texture_test->texture);
    ObjPoolCleanUp(&sprite_pool, (void **)&texture_test);
    DestroyObjPool(&sprite_pool);
    CloseWindow();
    return 0;
}

void UpdateDrawFrame()
{
    // TODO: call _process on each node
    texture_test->rotation++;

    // TODO: run physic engine
    
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexturePro(texture_test->texture, texture_test->clip, texture_test->dest, texture_test->center, (float)texture_test->rotation, WHITE);
        DrawLine((int)texture_test->dest.x, 0, (int)texture_test->dest.x, screenHeight, GRAY);
        DrawLine(0, (int)texture_test->dest.y, screenWidth, (int)texture_test->dest.y, GRAY);
        

        DrawText("Congrats! You created your second window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}

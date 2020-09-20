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

    //unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead);
    //void SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite);
    //char *LoadFileText(const char *fileName);
    //void SaveFileText(const char *fileName, char *text);
    //bool FileExists(const char *fileName)

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

Vector2 cursorPosition = { -100.0f, -100.0f };
Color cursorColor = DARKBLUE;

void UpdateDrawFrame()
{
    // Handle Events
    if (IsKeyDown(KEY_RIGHT)) texture_test->dest.x += 2.0f;
    if (IsKeyDown(KEY_LEFT)) texture_test->dest.x -= 2.0f;
    if (IsKeyDown(KEY_UP)) texture_test->dest.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN)) texture_test->dest.y += 2.0f;
    cursorPosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) cursorColor = MAROON;
    else if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) cursorColor = LIME;
    else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) cursorColor = DARKBLUE;

    // TODO: call _process on each node
    texture_test->rotation++;

    // TODO: run physic engine

    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexturePro(texture_test->texture, texture_test->clip, texture_test->dest, texture_test->center, (float)texture_test->rotation, WHITE);
        DrawLine((int)texture_test->dest.x, 0, (int)texture_test->dest.x, screenHeight, GRAY);
        DrawLine(0, (int)texture_test->dest.y, screenWidth, (int)texture_test->dest.y, GRAY);
        
        DrawText("Everythings' working fine so far...", 190, 200, 20, LIGHTGRAY);

        DrawCircleV(cursorPosition, 40, cursorColor);

    EndDrawing();
}

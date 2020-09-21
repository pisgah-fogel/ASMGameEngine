#define RMEM_IMPLEMENTATION // ObjPool
#define SUPPORT_TRACELOG
#define SUPPORT_TRACELOG_DEBUG
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "nodes.h"

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

node_base_t *sprite_test;

node_base_t *texture_test;

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

/**
 * @brief Same as sprites but does not rotate or get scaled
 */
typedef struct texture {
    Texture2D texture;
    int x;
    int y;
    Color tint;
} texture_t;

void _free_sprite(void *ptr) {
    // TODO: if debug enable check for pointer validity
    TraceLog(LOG_INFO, "Sprite unloaded");
    UnloadTexture(((sprite_t*) ptr)->texture);
    free(ptr); // Free our data
    ptr = NULL;
}

void _free_texture(void *ptr) {
    TraceLog(LOG_INFO, "Texture unloaded");
    UnloadTexture(((texture_t*) ptr)->texture);
    free(ptr); // Free our data
    ptr = NULL;
}

void _render_sprite(void *ptr) {
    sprite_t* sprite = (sprite_t*) ptr;
    DrawTexturePro(sprite->texture, sprite->clip, sprite->dest, sprite->center, (float)sprite->rotation, sprite->tint);
    DrawLine((int)sprite->dest.x, 0, (int)sprite->dest.x, 500, GRAY);
    DrawLine(0, (int)sprite->dest.y, 300, (int)sprite->dest.y, GRAY);
}

void _render_texture(void *ptr) {
    texture_t* tex = (texture_t*) ptr;
    DrawTexture(tex->texture, tex->x, tex->y, tex->tint);
}

void _process_sprite(void *ptr) {
    sprite_t* sprite = (sprite_t*) ptr;
    sprite->rotation++;
}

void _event_sprite(void *ptr) {
    sprite_t* sprite = (sprite_t*) ptr;
    if (IsKeyDown(KEY_RIGHT)) sprite->dest.x += 2.0f;
    if (IsKeyDown(KEY_LEFT)) sprite->dest.x -= 2.0f;
    if (IsKeyDown(KEY_UP)) sprite->dest.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN)) sprite->dest.y += 2.0f;
}

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

    sprite_test = (node_base_t*)malloc(sizeof(node_base_t));
    sprite_test->callback_free = &_free_sprite;
    sprite_test->callback_render = &_render_sprite;
    sprite_test->callback_process = &_process_sprite;
    sprite_test->callback_event = &_event_sprite;
    sprite_test->data = malloc(sizeof(sprite_t));
    
    ((sprite_t*)sprite_test->data)->texture = LoadTexture("resources/spritesheet.png");
    ((sprite_t*)sprite_test->data)->clip = (Rectangle){ 100.0f, 0.0f, 100.0f, 100.0f };
    ((sprite_t*)sprite_test->data)->dest = (Rectangle){ (float)screenWidth/2, (float)screenHeight/2, 100.0f, 100.0f };
    ((sprite_t*)sprite_test->data)->center = (Vector2){ 50.0f, 50.0f };
    ((sprite_t*)sprite_test->data)->rotation = 0;
    ((sprite_t*)sprite_test->data)->tint = WHITE;

    texture_test = (node_base_t*)malloc(sizeof(node_base_t));
    texture_test->callback_free = &_free_texture;
    texture_test->callback_render = &_render_texture;
    texture_test->data = malloc(sizeof(texture_t));
    ((texture_t*)texture_test->data)->texture = LoadTexture("resources/spritesheet.png");
    ((texture_t*)texture_test->data)->x = 0;
    ((texture_t*)texture_test->data)->y = 0;
    ((texture_t*)texture_test->data)->tint = (Color){0, 200, 200, 150};

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

    CloseWindow();
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

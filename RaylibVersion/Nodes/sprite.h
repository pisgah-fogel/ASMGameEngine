#pragma once

#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "Nodes/node.h"

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

void _free_sprite(void *arg) {
    TraceLog(LOG_INFO, "_free_sprite");
    sprite_t *ptr = ((node_base_t*)arg)->data;
    // TODO: if debug enable check for pointer validity
    UnloadTexture(ptr->texture);
    free(ptr); // Free our data
    ptr = NULL;
}

void _render_sprite(void *arg) {
    sprite_t *ptr = ((node_base_t*)arg)->data;
    DrawTexturePro(ptr->texture, ptr->clip, ptr->dest, ptr->center, (float)ptr->rotation, ptr->tint);
    DrawLine((int)ptr->dest.x, 0, (int)ptr->dest.x, 500, GRAY);
    DrawLine(0, (int)ptr->dest.y, 300, (int)ptr->dest.y, GRAY);
}

void _process_sprite(void *arg) {
    sprite_t *ptr = ((node_base_t*)arg)->data;
    ptr->rotation++;
}

void _event_sprite(void *arg) {
    sprite_t *ptr = ((node_base_t*)arg)->data;
    if (IsKeyDown(KEY_RIGHT)) ptr->dest.x += 2.0f;
    if (IsKeyDown(KEY_LEFT)) ptr->dest.x -= 2.0f;
    if (IsKeyDown(KEY_UP)) ptr->dest.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN)) ptr->dest.y += 2.0f;
}

void _init_sprite(void *arg) {
    ((node_base_t*)arg)->data = malloc(sizeof(sprite_t));
    sprite_t *ptr = ((node_base_t*)arg)->data;
    ptr->texture = LoadTexture("resources/spritesheet.png");
    ptr->clip = (Rectangle){ 100.0f, 0.0f, 100.0f, 100.0f };
    // TODO: screenHeight screenWidth
    ptr->dest = (Rectangle){ 400.0f, 400.0f, 100.0f, 100.0f };
    ptr->center = (Vector2){ 50.0f, 50.0f };
    ptr->rotation = 0;
    ptr->tint = WHITE;
}

static node_base_t* create_sprite() {
    node_base_t* sprite_test = (node_base_t*)malloc(sizeof(node_base_t));
    sprite_test->callback_free = &_free_sprite;
    sprite_test->callback_render = &_render_sprite;
    sprite_test->callback_process = &_process_sprite;
    sprite_test->callback_event = &_event_sprite;
    sprite_test->callback_init = &_init_sprite;
    sprite_test->callback_ready = NULL;
    sprite_test->callback_exiting = NULL;
    sprite_test->data = NULL;
    return sprite_test;
}
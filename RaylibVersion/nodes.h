#pragma once

#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "rmem.h"

/**
 * @brief All nodes are converted to this structure before being cast to there original structure
 */
typedef struct node_base {
    unsigned int node_type; // make it equal to __COUNTER__ when creating a new node type
    // TODO: Add node's options
    // TODO: add canary to check struct's integrity (-DDEBUG) ?
} node_base_t;
static const unsigned int NODE_TYPE_BASE  = __COUNTER__;

/**
 * @brief Sprites can be rotated and scaled
 */
typedef struct sprite {
    unsigned int node_type;
    Texture2D texture;
    Rectangle clip;
    Rectangle dest;
    Vector2 center; // 0, 0 is the top left of the clipped texture
    int rotation;
    Color tint;
} sprite_t;
static const unsigned int NODE_TYPE_SPRITE = __COUNTER__;

/**
 * @brief Same as sprites but does not rotate or get scaled
 */
typedef struct texture {
    unsigned int node_type;
    Texture2D texture;
    int x;
    int y;
    Color tint;
} texture_t;
static const unsigned int NODE_TYPE_TEXTURE = __COUNTER__;

/**
 * @brief Free node and it's resources
 * 
 * @param ptr pointer to the node
 * 
 * Does not handle NULL pointers yet.
 */
void node_free(node_base_t *ptr)
{
    // TODO: if debug enable check for pointer validity
    if ( ptr->node_type == NODE_TYPE_SPRITE) {
        TraceLog(LOG_INFO, "Sprite unloaded");
        UnloadTexture(((sprite_t*) ptr)->texture);
    }
    else if ( ptr->node_type == NODE_TYPE_TEXTURE ) {
        TraceLog(LOG_INFO, "Texture unloaded");
        UnloadTexture(((texture_t*) ptr)->texture);
    }
    free(ptr);
    ptr = NULL;
}

void node_event(node_base_t *ptr)
{
    if ( ptr->node_type == NODE_TYPE_SPRITE) {
        sprite_t* sprite = (sprite_t*) ptr;
        if (IsKeyDown(KEY_RIGHT)) sprite->dest.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) sprite->dest.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) sprite->dest.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) sprite->dest.y += 2.0f;
    }
}

void node_process(node_base_t *ptr)
{
    if ( ptr->node_type == NODE_TYPE_SPRITE) {
        sprite_t* sprite = (sprite_t*) ptr;
        sprite->rotation++;
    }
}

void node_render(node_base_t *ptr)
{
    // TODO: if debug enable check for pointer validity
    if ( ptr->node_type == NODE_TYPE_SPRITE) {
        sprite_t* sprite = (sprite_t*) ptr;
        DrawTexturePro(sprite->texture, sprite->clip, sprite->dest, sprite->center, (float)sprite->rotation, sprite->tint);
        DrawLine((int)sprite->dest.x, 0, (int)sprite->dest.x, 500, GRAY);
        DrawLine(0, (int)sprite->dest.y, 300, (int)sprite->dest.y, GRAY);
    }
    else if ( ptr->node_type == NODE_TYPE_TEXTURE ) {
        texture_t* tex = (texture_t*) ptr;
        DrawTexture(tex->texture, tex->x, tex->y, tex->tint);
    }
}
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
    void(*callback_free)(struct node_base *);
    void(*callback_render)(struct node_base *);
    void(*callback_process)(struct node_base *);
    void(*callback_event)(struct node_base *);
    // TODO: USe __attribute__((__packed__)) if there is a problem
    // TODO: add canary to check struct's integrity (-DDEBUG) ?
} node_base_t;
static const unsigned int NODE_TYPE_BASE  = __COUNTER__;

/**
 * @brief Sprites can be rotated and scaled
 */
typedef struct sprite {
    unsigned int node_type;
    void(*callback_free)(node_base_t *);
    void(*callback_render)(node_base_t *);
    void(*callback_process)(node_base_t *);
    void(*callback_event)(node_base_t *);

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
    void(*callback_free)(node_base_t *);
    void(*callback_render)(node_base_t *);
    void(*callback_process)(node_base_t *);
    void(*callback_event)(node_base_t *);

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
    // TODO: handle NULL pointers
    if (ptr->callback_free)
        (*ptr->callback_free)(ptr);
    free(ptr);
    ptr = NULL;
}

void node_event(node_base_t *ptr)
{
    if (ptr->callback_event)
        (*ptr->callback_event)(ptr);
}

void node_process(node_base_t *ptr)
{
    if (ptr->callback_process)
        (*ptr->callback_process)(ptr);
}

void node_render(node_base_t *ptr)
{
    if (ptr->callback_render)
        (*ptr->callback_render)(ptr);
}
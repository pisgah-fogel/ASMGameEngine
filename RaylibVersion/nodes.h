#pragma once

#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "rmem.h"

/**
 * @brief All nodes are converted to this structure before being cast to there original structure
 */
typedef struct node_base {
    void(*callback_free)(void*);
    void(*callback_render)(void*);
    void(*callback_process)(void*);
    void(*callback_event)(void*);
    void* data; // Used by the node system's user to store it's variables...
    // TODO: Use __attribute__((__packed__)) if there is a problem
    // TODO: add canary to check struct's integrity (-DDEBUG) ?
} node_base_t;

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
        (*ptr->callback_free)(ptr->data);
    free(ptr);
    ptr = NULL;
}

void node_event(node_base_t *ptr)
{
    if (ptr->callback_event)
        (*ptr->callback_event)(ptr->data);
}

void node_process(node_base_t *ptr)
{
    if (ptr->callback_process)
        (*ptr->callback_process)(ptr->data);
}

void node_render(node_base_t *ptr)
{
    if (ptr->callback_render)
        (*ptr->callback_render)(ptr->data);
}
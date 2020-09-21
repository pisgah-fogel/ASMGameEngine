#pragma once

#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "rmem.h"

#include "list.h"

/**
 * @brief All nodes are converted to this structure before being cast to there original structure
 */
typedef struct node_base {
    void* data; // Used by the node system's user to store it's variables...
    void(*callback_free)(void*);
    void(*callback_init)(void*); // Called when the node is created
    void(*callback_ready)(void*); // Called once the node is in the tree
    void(*callback_exiting)(void*); // Called just before removing the node from the tree
    void(*callback_render)(void*);
    void(*callback_process)(void*);
    void(*callback_event)(void*);
    struct node_base* parent;
    list_t child; // Must be initialized to NULL
    unsigned int child_count;
    // TODO: Use __attribute__((__packed__)) if there is a problem
    // TODO: add canary to check struct's integrity (-DDEBUG) ?
} node_base_t;

void node_add_child(node_base_t *parent, node_base_t *child)
{
    list_append(&parent->child, child);
    child->parent = parent;
    parent->child_count++;

    if (child->callback_ready)
        (*child->callback_ready)(child);
}

/**
 * @brief Free node and it's resources
 * 
 * @param ptr pointer to the node
 * 
 * Does not handle NULL pointers yet.
 */
void node_free(node_base_t *ptr)
{
    if (ptr->callback_exiting)
        (*ptr->callback_exiting)(ptr);
    
    // Free all childs
    element_t* item = ptr->child;
    while (item != NULL) {
        node_free(item->data);
        item = item->next;
    }

    // Free the list
    list_clear(&ptr->child);

    // TODO: handle NULL pointers
    if (ptr->callback_free)
        (*ptr->callback_free)(ptr);
    free(ptr);
    ptr = NULL;
}

void node_init(node_base_t *ptr)
{
    if (ptr->callback_init)
        (*ptr->callback_init)(ptr);
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

typedef struct node_root {
    int screenWidth;
    int screenHeight;
    node_base_t* head; // First element of the node tree
} node_root_t;

node_root_t* node_root_init(int screenWidth, int screenHeight, const char* windowName) {
    InitWindow(screenWidth, screenHeight, windowName);
    node_root_t* ptr = (node_root_t*)malloc(sizeof(node_root_t));
    ptr->screenHeight = screenHeight;
    ptr->screenWidth = screenWidth;
    ptr->head = NULL;
    return ptr;
}

void node_root_set_head(node_root_t* ptr, node_base_t* head) {
    ptr->head = head;
    head->parent = NULL; // It's the "head" of the node tree, then it does not have parent
}

void node_root_free(node_root_t** ptr) {
    if ((*ptr)->head != NULL)
        node_free((*ptr)->head);
    free(*ptr);
    *ptr = NULL;

    CloseWindow();
}
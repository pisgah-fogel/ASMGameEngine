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
    void* data; // Store your datas there, all callbacks get it as parameter
    void(*callback_free)(void*); // Called to free all resources, do not access the node tree there
    void(*callback_init)(void*); // Called when the node is created
    void(*callback_ready)(void*); // Called once the node is in the tree
    void(*callback_exiting)(void*); // Called just before removing the node from the tree
    void(*callback_render)(void*); // Called every frame, draw there
    void(*callback_process)(void*); // Called before the frame (do your processing / graphics updates there)
    void(*callback_event)(void*); // Called to handle event

    struct node_base* parent; // The parent node, NULL if it is the 'head' of the node tree
    list_t child; // Must be initialized to NULL, contains a list of children (nodes too)
    unsigned int child_count;
    Vector3 position;// Position of the node, by default place it in the center of your node's graphics
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
 */
void node_free(node_base_t *ptr)
{
    if (ptr == NULL)
        return;

    if (ptr->callback_exiting)
        (*ptr->callback_exiting)(ptr);

    // Free all childs
    element_t* child_list_ptr = ptr->child;
    element_t* child_list_ptr_next = NULL;
    while (child_list_ptr != NULL) {
        child_list_ptr_next = child_list_ptr->next; // Childs will remove their entry in their parents, savinf the next one before they do
        node_free(child_list_ptr->data);
        child_list_ptr = child_list_ptr_next;
    }
    // Free the list
    list_clear(&ptr->child);
    
    // remove the node from it's parent
    if (ptr->parent != NULL) {
        //size_t status = list_remplace((ptr->parent->child), ptr, NULL); // This works too, but leave empty nodes childs in the parent's child list
        size_t status = list_remove_by_reference(&(ptr->parent->child), ptr);
        printf("node_free: list_remove_by_reference returned %d\n", status);
    }

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

void node_recursive_event(node_base_t *ptr)
{
    node_event(ptr);
    
    // Iterate through childs
    element_t* it;
    element_t* it_next = ptr->child;
    while(it_next != NULL) {
        it = it_next;
        it_next = it->next;
        if (it->data != NULL)
            node_recursive_event(it->data);
    }
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

void node_recursive_render(node_base_t *ptr)
{
    node_render(ptr);
    
    // Iterate through childs and render them
    element_t* it;
    element_t* it_next = ptr->child;
    while(it_next != NULL) {
        it = it_next;
        it_next = it->next;
        if (it->data != NULL)
            node_recursive_render(it->data);
    }
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

void node_root_render(node_root_t* ptr) {
    if (ptr->head != NULL) {
        // Iterate through childs and render them
        node_recursive_render(ptr->head);
    }
}

void node_root_event(node_root_t* ptr) {
    if (ptr->head != NULL) {
        node_recursive_event(ptr->head);
    }
}
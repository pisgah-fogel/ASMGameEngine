#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "list.h"

static constexpr unsigned int consthash(const char* str) 
{ 
    if (str != nullptr)
    {
        unsigned int h = 89;
        while (*str != 0)
        {
            h = h * 33 + (*str++);
        }
        return h;
    }
    else
    {
        return 0;
    }
}

// For performance it's more efficient to store it there than passing it to all functions as argument
static float delta_process; // Elapsed time between two node_root_process call

/**
 * @brief All nodes are converted to this structure before being cast to there original structure
 */
typedef struct node_base {
    unsigned int id = 0;
    void* data = NULL; // Store your datas there, all callbacks get it as parameter
    void(*callback_free)(struct node_base*) = NULL; // Called to free all resources, do not access the node tree there
    void(*callback_init)(struct node_base*) = NULL; // Called when the node is created
    void(*callback_ready)(struct node_base*) = NULL; // Called once the node is in the tree
    void(*callback_exiting)(struct node_base*) = NULL; // Called just before removing the node from the tree
    void(*callback_render)(struct node_base*) = NULL; // Called every frame, draw there
    void(*callback_process)(struct node_base*, float) = NULL; // Called before the frame (do your processing / graphics updates there)
    void(*callback_event)(struct node_base*) = NULL; // Called to handle event

    struct node_base* parent = NULL; // The parent node, NULL if it is the 'head' of the node tree
    list_t childs = NULL; // Must be initialized to NULL, contains a list of children (nodes too)
    unsigned int child_count = 0;
} node_base_t;

void node_init(node_base_t *ptr)
{
    if (ptr->callback_init)
        (*ptr->callback_init)(ptr);
}

void node_add_child(node_base_t *parent, node_base_t *child)
{
    node_init(child);
    list_append(&parent->childs, child);
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
 * TODO: Optimisation: do not call list_remove_by_reference when we are removing the parent (recursive call)
 */
void node_free(node_base_t *ptr)
{
    if (ptr == NULL)
        return;

    if (ptr->callback_exiting)
        (*ptr->callback_exiting)(ptr);

    // Free all childs
    element_t* child_list_ptr = ptr->childs;
    element_t* child_list_ptr_next = NULL;
    while (child_list_ptr != NULL) {
        child_list_ptr_next = child_list_ptr->next; // Childs will remove their entry in their parents, savinf the next one before they do
        node_free(child_list_ptr->data);
        child_list_ptr = child_list_ptr_next;
    }
    // Free the list
    list_clear(&ptr->childs);
    
    // remove the node from it's parent
    if (ptr->parent != NULL) {
        ptr->parent->child_count--;
        //size_t status = list_remplace((ptr->parent->childs), ptr, NULL); // This works too, but leave empty nodes childs in the parent's childs list
        size_t status = list_remove_by_reference(&(ptr->parent->childs), ptr);
        printf("node_free: list_remove_by_reference returned %zu\n", status);
    }

    // TODO: handle NULL pointers
    if (ptr->callback_free)
        (*ptr->callback_free)(ptr);
    free(ptr);
    ptr = NULL;
}

void node_recursive_init(node_base_t *ptr)
{
    node_init(ptr);
    
    // Iterate through childs
    element_t* it;
    element_t* it_next = ptr->childs;
    while(it_next != NULL) {
        it = it_next;
        it_next = it->next;
        if (it->data != NULL)
            node_recursive_init(it->data);
    }
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
    element_t* it_next = ptr->childs;
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
        (*ptr->callback_process)(ptr, delta_process);
}

void node_recursive_process(node_base_t *ptr)
{
    node_process(ptr);
    
    // Iterate through childs and render them
    element_t* it;
    element_t* it_next = ptr->childs;
    while(it_next != NULL) {
        it = it_next;
        it_next = it->next;
        if (it->data != NULL)
            node_recursive_process(it->data);
    }
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
    element_t* it_next = ptr->childs;
    while(it_next != NULL) {
        it = it_next;
        it_next = it->next;
        if (it->data != NULL)
            node_recursive_render(it->data);
    }
}

typedef struct node_root {
    int screenWidth = 800;
    int screenHeight = 600;
    node_base_t* head = NULL; // First element of the node tree
} node_root_t;
static node_root_t* root = NULL;

node_root_t* node_root_create(int screenWidth, int screenHeight, const char* windowName) {
    InitWindow(screenWidth, screenHeight, windowName);
    node_root_t* ptr = (node_root_t*)malloc(sizeof(node_root_t));
    ptr->screenHeight = screenHeight;
    ptr->screenWidth = screenWidth;
    ptr->head = NULL;
    return ptr;
}

void node_root_set_head(node_base_t* head) {
    // TODO: Do something if root-> already set
    node_init(head); // node_add_child is not going to be called on this one, need to call it here
    root->head = head;
    head->parent = NULL; // It's the "head" of the node tree, then it does not have parent
}

void node_root_free() {
    if (root->head != NULL)
        node_free(root->head);
    free(root);
    root = NULL;

    CloseWindow();
}

void node_root_render() {
    if (root != NULL) {
        // Iterate through childs and render them
        node_recursive_render(root->head);
    }
}

void node_root_event() {
    if (root->head != NULL) {
        node_recursive_event(root->head);
    }
}

/**
 * @brief Not usefull, _init is automatically called when node_add_child is called
 * 
 * @param ptr 
 */
void node_root_init() {
    if (root->head != NULL) {
        node_recursive_init(root->head);
    }
}

void node_root_process() {
    delta_process = GetFrameTime();
    if (root->head != NULL) {
        node_recursive_process(root->head);
    }
}

node_root_t* node_get_root(){
    return root;
}

void node_root_setup(){
    if (root == NULL)
    {
        printf("node_root_setup\n");
        root = node_root_create(800, 800, "Node Test");
    } else {
        printf("Error: node_root_setup called twice\n");
        exit(1);
    }
}
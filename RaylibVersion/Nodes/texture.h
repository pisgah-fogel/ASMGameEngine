#pragma once

#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "Nodes/node.h"

/**
 * @brief Same as sprites but does not rotate or get scaled
 */
typedef struct texture {
    Texture2D texture;
    int x;
    int y;
    Color tint;
} texture_t;

void _free_texture(node_base_t *self) {
    TraceLog(LOG_INFO, "_free_texture");
    texture_t *ptr = (texture_t*)self->data;
    UnloadTexture(ptr->texture);
    free(ptr); // Free our data
    ptr = NULL;
}

void _render_texture(node_base_t *self) {
    texture_t *ptr = (texture_t*)self->data;
    DrawTexture(ptr->texture, ptr->x, ptr->y, ptr->tint);
}

void _init_texture(node_base_t* self) {
    self->data = malloc(sizeof(texture_t));
    texture_t *ptr = (texture_t*)self->data;
    ptr->texture = LoadTexture("resources/spritesheet.png");
    ptr->x = 0;
    ptr->y = 0;
    ptr->tint = (Color){0, 200, 200, 150};
}

static node_base_t* create_texture() {
    node_base_t* texture_test = (node_base_t*)malloc(sizeof(node_base_t));
    texture_test->id = consthash("texture_test_1");
    printf("Texture test id: %d\n", texture_test->id);
    texture_test->callback_free = &_free_texture;
    texture_test->callback_render = &_render_texture;
    texture_test->callback_process = NULL;
    texture_test->callback_event = NULL;
    texture_test->callback_init = &_init_texture;
    texture_test->callback_ready = NULL;
    texture_test->callback_exiting = NULL;
    texture_test->data = NULL;
    texture_test->parent = NULL;
    texture_test->child = NULL;
    texture_test->child_count = 0;
    return texture_test;
}
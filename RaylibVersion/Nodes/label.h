#pragma once

#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "Nodes/node.h"

typedef struct label {
    Color color;
    int fontsize;
    Vector2 position;
    char* text;
    bool needToFreeText;
} label_t;

void _free_label(node_base_t *self) {
    TraceLog(LOG_INFO, "_free_label");
    label_t *ptr = (label_t*)self->data;
    if (ptr->needToFreeText)
        free(ptr->text);
    free(ptr);
    ptr = NULL;
}

void _render_label(node_base_t *self) {
    label_t *ptr = (label_t*)self->data;
    DrawText(ptr->text, ptr->position.x, ptr->position.y, ptr->fontsize, ptr->color);
}

void _init_label(node_base_t *self) {
    self->data = malloc(sizeof(label_t));
    label_t *ptr = (label_t*)self->data;
    ptr->text = "This is an awesome test Label";
    ptr->needToFreeText = false;
    ptr->color = LIGHTGRAY;
    ptr->fontsize = 30;
    ptr->position = (Vector2){50, 300};
}

static node_base_t* create_label() {
    node_base_t* label_test = (node_base_t*)malloc(sizeof(node_base_t));
    *label_test = (node_base_t){};
    label_test->id = consthash("label_test_1");
    printf("Label test id: %u\n", label_test->id);
    label_test->callback_free = &_free_label;
    label_test->callback_render = &_render_label;
    label_test->callback_init = &_init_label;
    return label_test;
}
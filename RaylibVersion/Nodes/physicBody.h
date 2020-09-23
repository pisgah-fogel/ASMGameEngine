#pragma once

#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "Nodes/node.h"

typedef struct physicBody {
} physicBody_t;

void _free_physicBody(node_base_t *self) {
    TraceLog(LOG_INFO, "_free_physicBody");
    physicBody_t *ptr = (physicBody_t*)self->data;
    // TODO: free other things there
    free(ptr);
    ptr = NULL;
}

void _render_physicBody(node_base_t *self) {
    physicBody_t *ptr = (physicBody_t*)self->data;
    // TODO: draw here
}

void _init_physicBody(node_base_t *self) {
    self->data = malloc(sizeof(physicBody_t));
    physicBody_t *ptr = (physicBody_t*)self->data;
    // TODO: init
}

static node_base_t* create_physicBody() {
    node_base_t* physicBody_test = (node_base_t*)malloc(sizeof(node_base_t));
    *physicBody_test = (node_base_t){};
    physicBody_test->id = consthash("physicBody_test_1");
    printf("physicBody test id: %u\n", physicBody_test->id);
    physicBody_test->callback_free = &_free_physicBody;
    physicBody_test->callback_render = &_render_physicBody;
    physicBody_test->callback_init = &_init_physicBody;
    return physicBody_test;
}
#pragma once

#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "Nodes/node.h"

typedef struct physicBody {
    PhysicsBody circle;
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
    //ptr->circle->position.x
    //ptr->circle->position.y
    if (IsKeyDown(KEY_A)) ptr->circle->enabled = true;
    int vertexCount = ptr->circle->shape.vertexData.vertexCount;
    for (int j = 0; j < vertexCount; j++)
    {
        // Get physics bodies shape vertices to draw lines
        // Note: GetPhysicsShapeVertex() already calculates rotation transformations
        Vector2 vertexA = GetPhysicsShapeVertex(ptr->circle, j);
        int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
        Vector2 vertexB = GetPhysicsShapeVertex(ptr->circle, jj);
        DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
    }
}

void _init_physicBody(node_base_t *self) {
    self->data = malloc(sizeof(physicBody_t));
    physicBody_t *ptr = (physicBody_t*)self->data;

    ptr->circle = CreatePhysicsBodyCircle((Vector2){ 200, -400 }, 45, 1);
    ptr->circle->enabled = false;
}

static node_base_t* create_physicBody() {
    node_base_t* physicBody_test = (node_base_t*)malloc(sizeof(node_base_t));
    *physicBody_test = node_base_t_default;
    physicBody_test->id = consthash("physicBody_test_1");
    printf("physicBody test id: %u\n", physicBody_test->id);
    physicBody_test->callback_free = &_free_physicBody;
    physicBody_test->callback_render = &_render_physicBody;
    physicBody_test->callback_init = &_init_physicBody;
    return physicBody_test;
}
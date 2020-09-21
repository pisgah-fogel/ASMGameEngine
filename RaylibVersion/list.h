/**
 * @file list.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-09-21
 * 
 * @copyright Copyright (c) 2020
 * 
 * Exemple:
    #include "list.h"
    printf("Test list\n");
    list_t l = list_create();
    printf("Empty list created\n");
    list_clear(&l);
    printf("Empty list cleared\n");
    printf("Empty list size = %d\n", list_size(l));
    list_append(&l, 42);
    printf("Append to list\n");
    printf("1 element list size = %d\n", list_size(l));
    list_append(&l, 10);
    list_append(&l, 7);
    printf("3 element list size = %d\n", list_size(l));

    printf("list get element 0 (7 expected) = %d\n", list_get(l, 0));
    printf("list get element 1 (10 expected) = %d\n", list_get(l, 1));
    printf("list get element 2 (42 expected) = %d\n", list_get(l, 2));
    printf("list get element 3 (-1 expected) = %d\n", list_get(l, 3));

    list_clear(&l);
    printf("List cleared\n");
    printf("Cleared list size = %d\n", list_size(l));
 */

#pragma once

#include <stdlib.h>

#include "Nodes/node.h"

#define LIST_TYPE node_base_t*
#define LIST_NIL NULL

typedef struct node_base node_base_t;

typedef struct element
{
    LIST_TYPE data;
    struct element* next;
} element_t;

typedef element_t* list_t;

/**
 * @brief Append some data to the list
 * 
 * @param list pointer to the list (element_t**)
 * @param data data of type LIST_TYPE to append to the list
 */
void list_append(list_t *list, LIST_TYPE data) {
    element_t* elm = (element_t*)malloc(sizeof(element_t));
    elm->data = data;
    elm->next = *list;
    *list = elm;
}

/**
 * @brief If LIST_TYPE is a pointer, this function does not free it
 * 
 * @param list pointer to the list (element_t**)
 */
void list_clear(list_t *list) {
    element_t* ptr;
    element_t* ptr_next = *list;
    while (ptr_next != NULL) {
        ptr = ptr_next;
        ptr_next = ptr->next;
        free(ptr);
    }
    *list = NULL;
}

list_t list_create() {
    return NULL;
}

size_t list_size(const list_t list) {
    element_t* ptr = list;
    size_t i = 0;
    while(ptr!=NULL) {
        ptr = ptr->next;
        i++;
    }
    return i;
}

LIST_TYPE list_get(const list_t list, size_t index) {
    element_t* ptr = list;

    size_t i = index;
    while( i != 0 ) {
        if (ptr == NULL)
            return LIST_NIL;
        ptr = ptr->next;
        i--;
    }
    if (ptr == NULL)
        return LIST_NIL; // Empty list
    return ptr->data;
}
//
// Created by Yu Liang on 27/7/21.
//
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "list.h"

typedef struct List_Repr {
    struct Node *head;
    size_t length;
    struct Node *tail;
} List_Repr;

typedef struct Node {
    string data;
    struct Node *next;
    struct Node *prev;
} Node;

// meta interface
/**
 * list_create
 * allocate the required memory for a new list
 * return a pointer to the new list
 * return NULL on error
 */
list list_create (void) {
    list new_list = malloc(sizeof(*new_list));
    if (!new_list) return NULL;
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->length = 0;
    return new_list;
}
/**
 * list_destroy
 * free all memory associated with a given list
 */
void list_destroy (list list) {
    assert(list);
    if (list->head != NULL) {
        Node *no_cur = list->head;
        Node *no_nex = NULL;
        while (no_cur != NULL) {
            no_nex = no_cur->next;
            free(no_cur->data);
            free(no_cur);
            no_cur = no_nex;
        }
        free(list);
    }
}

// misc interface
/**
 * list_is_empty
 * return True if there are no items in the list, False otherwise
 * return False on error
 */
bool list_is_empty (list list) {
    if (!list) return false;

    if (list->length != 0) {
        return true;
    } else {
        return false;
    }
}
/**
 * list_length
 * return the number of items in the list
 * return 0 on error
 */
size_t list_length (list list) {
    if (!list) return false;

    return list->length;
}

// stack interface
/**
 * list_push
 * add an item to the head of the stack
 */
void list_push (list list,string string) {
    assert(list);
    Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->prev = NULL;
    new->data = strdup(string);
    if (!list->head) {
        list->tail = list->head = new;
    } else {
        new->next = list->head;
        list->head->prev = new;
        list->head = new;
    }
    list->length++;
}
/**
 * list_pop
 * remove and return the item at the head of the stack
 * return NULL on error
 */
string list_pop (list list) {
    assert(list);
    if (list->head) {
        Node *first;
        first = list->head;
        if (list->tail == list->head) {
            free(list->head);
            list->tail = list->head = NULL;
            list->length--;
            return first->data;
        } else {
            list->head = list->head->next;
            free(list->head->prev;
            list->head->prev= NULL;
            list->length--;
            return first->data;
        }
    } else {
        return NULL;
    }
}

// queue interface
/**
 * list_enqueue
 * add an item to the head of the queue
 */
void list_enqueue (list list,string string) {
    assert(list);
    Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->prev = NULL;
    new->data = strdup(string);
    if (!list->head) {
        list->tail = list->head = new;
    } else {
        new->next = list->head;
        list->head->prev = new;
        list->head = new;
    }
    list->length++;
}
/**
 * list_dequeue
 * remove and return the item at the tail of the queue
 * return NULL on error
 */
string list_dequeue (list list) {
    assert(list);
    if (list->tail) {
        Node *last;
        last = list->tail;
        if (list->tail == list->head) {
            free(list->tail);
            list->tail = list->head = NULL;
            list->length--;
            return last->data;
        } else {
            list->tail = list->tail->prev;
            free(list->tail->next);
            list->tail->next = NULL;
            list->length--;
            return last->data;

        }
    } else {
        return NULL;
    }
}

// set interface
/**
 * list_add
 * add an item into the set, if it is not already in the set
 */
void list_add (list list, string string) {
    assert(list);
    Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->prev = NULL;
    new->data = strdup(string);
    if (list->head) {
        Node *p = list->head;
        while(p && strcmp(p->data, string) == 0) {
            p = p->next;
        }
        if (!p) {
            list->tail->next = new;
            new->prev = list->tail;
            list->tail = new;
            list->length++;
        } else {
            free(new);
            return;
        }
    } else {
        list->head = list->tail = new;
        list->length++;
    }
}
/**
 * list_remove
 * remove an item from the set
 */
void list_remove (list list,string string) {
    assert(list);
    if (list->head) {
        Node *p = list->head;
        while(p && strcmp(p->data, string) == 0) {
            p = p->next;
        }

        if (p && p == list->head) {
            list->head = list->head->next;
            free(p);
            list->head->prev = NULL;
            list->length--;
        } else if (p && p == list->tail) {
            list->tail = list->tail->prev;
            free(p);
            list->tail->next = NULL;
            list->length--;
        } else if (!p){
            return;
        } else {
            p->next->prev = p->prev;
            p->prev->next = p->next;
            free(p);
            list->length--;
        }
    } else {
        return;
    }
}
/**
 * list_contains
 * return True if a particular item is in the set, False otherwise
 * return False on error
 */
bool list_contains (list list, string string) {
    assert(list);
    if (list->head) {
        Node *p = list->head;
        while(p && strcmp(p->data, string) == 0) {
            p = p->next;
        }
        if (p) {
            return true;
        } else {
            return false;
        }
    }
}

// general interface
// You do not need to implement these functions, but they could be useful internally or for testing...
//void   list_append   (list, string);
//string list_shrink   (list);
//void   list_prepend  (list, string);
//string list_shift    (list);

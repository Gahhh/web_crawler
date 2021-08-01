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
void list_destroy (list ddlist) {
    if (ddlist->head) {
        Node *no_cur = ddlist->head;
        while (no_cur) {
            Node *temp = no_cur;
            no_cur = no_cur->next;
            free(temp->data);
            free(temp);

        }
        ddlist->length = 0;
        free(ddlist);
    }
}

// misc interface
/**
 * list_is_empty
 * return True if there are no items in the list, False otherwise
 * return False on error
 */
bool list_is_empty (list ddlist) {
    if (!ddlist) return false;
    if (ddlist->length == 0) {
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
size_t list_length (list ddlist) {
    if (!ddlist) return false;

    return ddlist->length;
}

// stack interface
/**
 * list_push
 * add an item to the head of the stack
 */
void list_push (list ddlist, string value) {
    Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->prev = NULL;
    new->data = strdup(value);
    if (!ddlist->head) {
        ddlist->tail = ddlist->head = new;
    } else {
        new->next = ddlist->head;
        ddlist->head->prev = new;
        ddlist->head = new;
    }
    ddlist->length++;
}
/**
 * list_pop
 * remove and return the item at the head of the stack
 * return NULL on error
 */
string list_pop (list ddlist) {
    if (ddlist->head) {
        Node *first;
        first = ddlist->head;
        if (ddlist->tail == ddlist->head) {
            free(ddlist->head);
            ddlist->tail = ddlist->head = NULL;
            ddlist->length--;
            return first->data;
        } else {
            ddlist->head = ddlist->head->next;
            free(ddlist->head->prev);
            ddlist->head->prev= NULL;
            ddlist->length--;
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
void list_enqueue (list ddlist,string value) {
    Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->prev = NULL;
    new->data = strdup(value);
    if (!ddlist->head) {
        ddlist->tail = ddlist->head = new;
    } else {
        new->next = ddlist->head;
        ddlist->head->prev = new;
        ddlist->head = new;
    }
    ddlist->length++;
}
/**
 * list_dequeue
 * remove and return the item at the tail of the queue
 * return NULL on error
 */
string list_dequeue (list ddlist) {
    if (ddlist->tail) {
        string data = ddlist->tail->data;
        if (ddlist->tail == ddlist->head) {
            free(ddlist->tail);
            ddlist->tail = ddlist->head = NULL;
            ddlist->length--;
            return data;
        } else {
            ddlist->tail = ddlist->tail->prev;
            free(ddlist->tail->next);
            ddlist->tail->next = NULL;
            ddlist->length--;
            return data;

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
void list_add (list ddlist, string value) {
    Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->prev = NULL;
    new->data = strdup(value);
    if (ddlist->head) {
        Node *p = ddlist->head;
        while(p && strcmp(p->data, value) != 0) {
            p = p->next;
        }
        if (!p) {
            ddlist->tail->next = new;
            new->prev = ddlist->tail;
            ddlist->tail = new;
            ddlist->length++;
        } else {
            free(new);
            return;
        }
    } else {
        ddlist->head = ddlist->tail = new;
        ddlist->length++;
    }
}
/**
 * list_remove
 * remove an item from the set
 */
void list_remove (list ddlist,string value) {
    if (ddlist->head) {
        Node *p = ddlist->head;
        while(p && strcmp(p->data, value) != 0) {
            p = p->next;
        }

        if (p && p == ddlist->head) {
            ddlist->head = ddlist->head->next;
            free(p->data);
            free(p);
            ddlist->head->prev = NULL;
            ddlist->length--;
        } else if (p && p == ddlist->tail) {
            ddlist->tail = ddlist->tail->prev;
            free(p->data);
            free(p);
            ddlist->tail->next = NULL;
            ddlist->length--;
        } else if (!p){
            return;
        } else {
            p->next->prev = p->prev;
            p->prev->next = p->next;
            free(p->data);
            free(p);
            ddlist->length--;
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
bool list_contains (list ddlist, string value) {
    if (ddlist->head) {
        Node *p = ddlist->head;
        while(p && strcmp(p->data, value) != 0) {
            p = p->next;
        }
        if (p) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// general interface
// You do not need to implement these functions, but they could be useful internally or for testing...
//void   list_append   (list, string);
//string list_shrink   (list);
//void   list_prepend  (list, string);
//string list_shift    (list);

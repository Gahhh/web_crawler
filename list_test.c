//
// Created by Yu Liang on 28/7/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    list test1 = list_create();
    printf("list should be empty: %s", list_is_empty(test1) ? "yes" : "no");
    // stack
    char *name = "abc";
    for (int i=0; i<=10; i++) {
        list_push(test1, name);
    }
    if (test1 != NULL) {
        Node *p;
        for (p=test1->head; p != NULL; p=p->next) {
            printf("%s\n", p->data);
        }
    }
    return 0;
}
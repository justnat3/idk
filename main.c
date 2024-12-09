// 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LINE_MAX 256

typedef struct list_head {
    int pos;
    int data;
    struct list_head *next;
    struct list_head *prev;
    bool chosen;
} list_head;

int list_cmp(int id, list_head*loc);
list_head* list_add(int id, list_head*loc);
list_head *list_start(list_head* list);
int list_len(list_head* loc);
bool is_whitespace(char s);
list_head *smallest(list_head*loc);

int main() {
    FILE *fptr = fopen("./sample.txt", "r");
    if (fptr == NULL) {
        printf("d1: could not open sample data");
        return -1;
    }

    list_head *first = NULL;
    list_head *second = NULL;
    char ln[LINE_MAX];

    int c;
    do {
        unsigned long len = strlen(ln);
        if (len <= 1) {continue;}
        char f= ln[0];
        char s= ln[len-2];

        first = list_add(f-'0', first);
        second = list_add(s-'0', second);
    } while(fgets(ln, sizeof(ln), fptr));

    int first_len = list_len(first);
    if (first_len != list_len(second)) {
        printf("INVALID LENGTH\n");
        return 1;
    }

    for (int i = 0; i < first_len; ++i) {
        list_head *fnext_small = smallest(first);
        list_head *snext_small = smallest(first);
        int small = fnext_small->data = snext_small->data;
        // if (small < 0) {small = -1*small;}
        printf("distance: %d\n", small);
    }

}

list_head *list_start(list_head* list) {
    while (list->prev != NULL) {
        list = list->prev;
    }
    return list;
}

int list_cmp(int id, list_head*loc) {
    if (loc->data < id) return true;
    return false;
}

list_head *list_add(int data, list_head *loc) {
    int npos = 0;

    if (loc == NULL) {
        list_head *loc = malloc(sizeof(list_head));
        loc->pos = 0;
        loc->data = data;
        loc->prev = NULL;
        loc->next = NULL;
        loc->chosen = false;
        return loc;
    }

    list_head *l = malloc(sizeof(list_head));
    if (l == NULL) {exit(1);}
    loc->next = l;

    l->data = data;
    l->pos = loc->pos + 1;
    l->prev = loc;
    l->next = NULL;
    l->chosen = false;
    return l;
}

int list_len(list_head* loc) {
    loc = list_start(loc);
    while (loc->next != NULL){
        loc = loc->next;
    };
    return loc->pos;
}

// NOTE(nate): consistently remove list nodes with the data of the last smallest
list_head *list_remove_with_data(int data, list_head*loc);

list_head *smallest(list_head*loc) {
    loc = list_start(loc);
    int start = true;
    int small_pos = 0;
    int small = -1;
    while (loc->next != NULL){
        if (start) {
            small = loc->data;
            start = false;
            continue;
        }

        if (loc->data < small) {
            small = loc->data;
            small_pos = loc->pos;
        }

        loc = loc->next;
    };

    // this is out of laziness
    loc = list_start(loc);
    while (loc->next != NULL) {
        if (loc->pos == small_pos) {loc->chosen = true; break;}
        loc = loc->next;
    }

    return loc;
}

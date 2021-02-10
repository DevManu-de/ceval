
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "xmalloc.h"

void init_nodes(node *first);

calcualtion *create_calculation(int size) {

    calcualtion *calc = xmalloc(sizeof(calcualtion));
    calc->length = size;

    node *n = xmalloc(sizeof(node));

    calc->first = n;
    n->prev = NULL;

    node *p;

    while (--size) {

        n->next = xmalloc(sizeof(node));
        p = n;
        n = n->next;
        n->prev = p;
    }
    n->next = NULL;

    calc->last = n;

    return calc;


}

char *format_text(char *text) {


    int tlen = strlen(text);

    char* format_text = xmalloc(sizeof(char) * tlen + 1);
    int ftlen = 0;

    int i;
    for (i = 0; i < tlen; ++i)
        if (strchr(VALID_CHARS, text[i]))
            format_text[ftlen++] = text[i];

    format_text = xrealloc(format_text, strlen(format_text) + 1);
    return format_text;

}

void free_calculation(calcualtion *calc) {

    node *n = calc->first;
    node *x;
    while (n != NULL) {
        x = n->next;
        xfree(n);
        n = x;

    }

    xfree(calc);

}

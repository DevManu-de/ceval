
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "xmalloc.h"

double *strtofpntr(char *str);

calcualtion *create_calculation(int size) {

    calcualtion *calc = xmalloc(sizeof(calcualtion));
    calc->length = size;

    node *n = xmalloc(sizeof(node));

    calc->first = n;
    n->type = 0;
    n->prev = NULL;

    node *p;

    while (--size) {

        n->next = xmalloc(sizeof(node));
        p = n;
        n = n->next;
        n->type = 0;
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

    format_text[ftlen] = '\0';
    format_text = xrealloc(format_text, strlen(format_text) + 1);

    return format_text;

}

void init_calculation(calcualtion *calc, char *format_text) {

    char *op;

    node *n = calc->first;
    while ((op = strpbrk(format_text, VALID_OPERATORS)) != NULL) {

        int len = op - format_text;

        if (len == 0) {
            n->item = strndup(format_text, 1);
            n->type = 2;
            ++format_text;
        } else {
            n->item = strtofpntr(format_text);
            n->type = 1;
            format_text += len;
        }
        n = n->next;
    }

    if (format_text[0] != '\0') {
        n->item = strtofpntr(format_text);
        n->type = 1;
    }

}

void free_calculation(calcualtion *calc) {

    node *n = calc->first;
    node *x;
    while (n != NULL) {
        x = n->next;
        if (n->type != 0)
            free(n->item);
        xfree(n);
        n = x;

    }

    xfree(calc);

}

double *strtofpntr(char *str) {

    double f = strtof(str, NULL);
    double *fpntr = xmalloc(sizeof(double));
    fpntr = memmove(fpntr, &f, sizeof(double));

    return fpntr;
}

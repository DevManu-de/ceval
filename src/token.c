#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "xmalloc.h"

void calculation_append_node(calcualtion *calc, void *item, unsigned int type);
void solve_calculation_bracket_pair(calcualtion *calc, node *open, node *close);
void solve_calculation_range(calcualtion *calc, node *start, node *end);
double *strtofpntr(char *str);
void free_node(calcualtion *calc, node *n);

calcualtion *create_calculation() {

    calcualtion *calc = xmalloc(sizeof(calcualtion));
    calc->length = 0;
    calc->result = 0.0;

    calc->first = NULL;
    calc->last = NULL;

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

void calculation_append_node(calcualtion *calc, void *item, unsigned int type) {


    node *n = xmalloc(sizeof(node));
    n->item = item;
    n->type = type;
    n->next = NULL;

    if (calc->first == NULL) {

        calc->first = n;
        calc->last = n;
        n->prev = NULL;

    } else {

        n->prev = calc->last;
        calc->last->next = n;
        calc->last = n;

    }

}

void init_calculation(calcualtion *calc, char *format_text) {

    char *op;

    while ((op = strpbrk(format_text, VALID_OPERATORS)) != NULL) {

        int len = op - format_text;

        if (len == 0) {
            calculation_append_node(calc, strndup(format_text, 1), IS_OPERATOR);
            ++format_text;
        } else {
            calculation_append_node(calc, strtofpntr(format_text), IS_NUMBER);
            format_text += len;
        }
    }

    if (format_text[0] != '\0') {
        calculation_append_node(calc, strtofpntr(format_text), IS_NUMBER);
    }

}

void prepare_calculation(calcualtion *calc) {

    node *n = calc->first;
    node *x;

    while (n != NULL) {
        x = n->next;
        if (n->type == IS_OPERATOR && ((char *) n->item)[0] == '-') {
            if (n->next->type == IS_NUMBER) {
                ((double *) n->next->item)[0] *= -1.0;
                free_node(calc, n);

            }

        }
        n = x;
    }

}

void solve_calculation(calcualtion *calc) {



}

void solve_calculation_bracket_pair(calcualtion *calc, node *open, node *close) {


}

void solve_calculation_range(calcualtion *calc, node *start, node *end) {


}

void free_calculation(calcualtion *calc) {

    node *n = calc->first;
    node *x;
    while (n != NULL) {
        x = n->next;
        xfree(n->item);
        xfree(n);
        n = x;

    }

    xfree(calc);

}

void free_node(calcualtion *calc, node *n) {

    if (calc->first == n) {

        calc->first = n->next;
        xfree(n->item);
        xfree(n);
        calc->first->prev = NULL;

    } else if (calc->last == n) {

        calc->last = n->prev;
        xfree(n->item);
        xfree(n);
        calc->last->next = NULL;

    } else {

        n->prev->next = n->next;
        n->next->prev = n->prev;
        xfree(n->item);
        xfree(n);

    }

}

double *strtofpntr(char *str) {

    double f = strtof(str, NULL);
    double *fpntr = xmalloc(sizeof(double));
    fpntr = memmove(fpntr, &f, sizeof(double));

    return fpntr;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "xmalloc.h"

void calculation_append_node(calcualtion *calc, void *item, unsigned int type);
void solve_calculation_bracket_pair(calcualtion *calc, node *open, node *close);
void solve_calculation_range(calcualtion *calc, node *start, node *end);
void solve_division(calcualtion *calc, node *num1, node *num2);
void solve_multiplication(calcualtion *calc, node *num1, node *num2);
void solve_addition(calcualtion *calc, node *num1, node *num2);
void solve_substraction(calcualtion *calc, node *num1, node *num2);
node *find_node(calcualtion *calc, void *item, int type, int direction);
double *strtofpntr(char *str);
void free_node(calcualtion *calc, node *n);

void print_calculator(calcualtion *calc);

calcualtion *create_calculation() {

    calcualtion *calc = xmalloc(sizeof(calcualtion));
    calc->result = 0.0;
    calc->length = 0;

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
    ++calc->length;

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

void format_calculation(calcualtion *calc) {

    node *n = calc->first;
    node *x;

    while (n != NULL) {
        x = n->next;
        if (n->type == IS_OPERATOR && ((char *) n->item)[0] == '-') {
            if (n->next->type == IS_NUMBER) {
                ((double *) n->next->item)[0] *= -1.0;
                free_node(calc, n);

            }

        } else if (n->type == IS_OPERATOR && ((char *) n->item)[0] == '+') {
            if (n->next->type == IS_NUMBER) {
                free_node(calc, n);
            }
        }
        n = x;
    }

}

void solve_calculation(calcualtion *calc) {

    node *open;
    node *close;

    while ((open = find_node(calc, "(", IS_OPERATOR, BACKWARDS)) != NULL && (close = find_node(calc, ")", IS_OPERATOR, FORWARD)) != NULL) {
        solve_calculation_bracket_pair(calc, open, close);
    }

    solve_calculation_range(calc, calc->first, calc->last);
    calc->result = ((double *) calc->first->item)[0];

}

void solve_calculation_bracket_pair(calcualtion *calc, node *open, node *close) {

    node *start = open->next;
    node *end = close->prev;
    free_node(calc, open);
    free_node(calc, close);
    if (start != end) {
        solve_calculation_range(calc, start, end);
    }
    format_calculation(calc);

}

void solve_calculation_range(calcualtion *calc, node *start, node *end) {

    calcualtion *c = xmalloc(sizeof(calcualtion));
    c->first = start;
    c->last = end;
    node *afterend = end->next;
    end->next = NULL;

    int syncfirsts = 0;
    int synclasts = 0;

    if (c->first == calc->first)
        syncfirsts = 1;
    if (c->last == calc->last)
        synclasts = 1;

    node *n;
    while ((n = find_node(c, "/", IS_OPERATOR, FORWARD)) != NULL) {
        solve_division(c, n->prev, n->next);
    }

    while ((n = find_node(c, "*", IS_OPERATOR, FORWARD)) != NULL) {
        solve_multiplication(c, n->prev, n->next);

    }

    n = c->first;
    while (n != NULL && n->type == IS_NUMBER && n->next != NULL && n->next->type == IS_NUMBER) {

        solve_addition(c, n, n->next);
    }

    if (syncfirsts)
        calc->first = c->first;
    if (synclasts)
        calc->last = c->last;

    c->last->next = afterend;
    if (afterend != NULL) {
        c->last->next->prev = c->last;
    }
    xfree(c);

}

void solve_division(calcualtion *calc, node *num1, node *num2) {

    double product = ((double *) num1->item)[0] / ((double *) num2->item)[0];

    node *operator = num1->next;
    xfree(operator->item);

    operator->item = xmalloc(sizeof(double));
    memmove(operator->item, &product, sizeof(double));

    operator->type = IS_NUMBER;

    free_node(calc, num1);
    free_node(calc, num2);

}

void solve_multiplication(calcualtion *calc, node *num1, node *num2) {

    double product = ((double *) num1->item)[0] * ((double *) num2->item)[0];

    node *operator = num1->next;
    xfree(operator->item);

    operator->item = xmalloc(sizeof(double));
    memmove(operator->item, &product, sizeof(double));

    operator->type = IS_NUMBER;

    free_node(calc, num1);
    free_node(calc, num2);

}

void solve_addition(calcualtion *calc, node *num1, node *num2) {

    double sum = ((double *) num1->item)[0] + ((double *) num2->item)[0];
    xfree(num1->item);

    num1->item = xmalloc(sizeof(double));
    memmove(num1->item, &sum, sizeof(double));

    num1->type = IS_NUMBER;
    free_node(calc, num2);

}

node *find_node(calcualtion *calc, void *item, int type, int direction) {

    node *n;

    if (direction == FORWARD) {
        n = calc->first;

        while (n != NULL) {
            if (memcmp(n->item, item, type == IS_NUMBER ? sizeof(double) : 1) == 0) {
                return n;
            }
            n = n->next;
        }
        return NULL;

    } else {
        n = calc->last;
        while (n != NULL) {
            if (memcmp(n->item, item, type == IS_NUMBER ? sizeof(double) : 1) == 0) {
                return n;
            }
            n = n->prev;
        }
        return NULL;

    }
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

    --calc->length;

}

double *strtofpntr(char *str) {

    double f = strtof(str, NULL);
    double *fpntr = xmalloc(sizeof(double));
    fpntr = memmove(fpntr, &f, sizeof(double));

    return fpntr;
}

void print_calculator(calcualtion *calc) {

    puts("\n\n\n");

    node *n = calc->first;
    while (n != NULL) {
        if (n->type == IS_NUMBER)
            printf("%f\n", ((double *) n->item)[0]);
        else
            puts(n->item);
        n = n->next;
    }

    puts("\n\n\n");
}

#include "ceval.h"

#include "doublist.h"
#include "xmemtools.h"

#define VALID_CHARS "0123456789+-*/()."
#define VALID_NUMBERS "0123456789"
#define VALID_OPERATORS "+-*/()"

#define IS_NUMBER 1
#define IS_OPERATOR 2
#define IS_NULL 0

void print_calculation(struct doublist *list);

char *format_text(char *text);
void init_calculation(struct doublist *calc, char *formatted_test);
void format_calculation(struct doublist *calc);

void solve_calculation(struct doublist *calc);
void solve_calculation_bracket_pair(struct doublist *calc, struct node *open, struct node *close);
void solve_calculation_range(struct doublist *calc, struct node *start, struct node *end);
void solve_operation(struct doublist *calc, struct node **start, struct node *op, struct node **end);

double *strtodpntr(char *str);

double ceval(char *_calc_) {

    struct doublist *calc = doublist_create();
    char *formatted_test = format_text(_calc_);
    init_calculation(calc, formatted_test);
    xfree(formatted_test);
    solve_calculation(calc);

    double solution = ((double *) calc->head->value)[0];

    doublist_free(calc);

    return solution;

}

char *format_text(char *text) {

    unsigned int tlen = strlen(text);

    char formatted_test[tlen + 1];
    unsigned int ftlen = 0;

    int i;
    for (i = 0; i < tlen; ++i) {
        if (strchr(VALID_CHARS, text[i])) {
            formatted_test[ftlen++] = text[i];
        }

    }

    formatted_test[ftlen] = '\0';

    return strdup(formatted_test);

}

void init_calculation(struct doublist *calc, char *formatted_test) {

    char *op;

    while ((op = strpbrk(formatted_test, VALID_OPERATORS)) != NULL) {

        int len = op - formatted_test;

        if (len == 0) {
            node_insert_after(calc, calc->tail, node_create(strndup(op, 1), IS_OPERATOR));
            ++formatted_test;

        } else {
            node_insert_after(calc, calc->tail, node_create(strtodpntr(formatted_test), IS_NUMBER));
            formatted_test += len;

        }

    }

    if (formatted_test[0] != '\0') {
        node_insert_after(calc, calc->tail, node_create(strtodpntr(formatted_test), IS_NUMBER));
    }

}

void solve_calculation(struct doublist *calc) {

    struct node *open;
    struct node *close;

    while ((close = node_find(calc, calc->head, ")", IS_OPERATOR, 1, FORWARD)) && (open = node_find(calc, close, "(", IS_OPERATOR, 1, BACKWARD))) {
        solve_calculation_bracket_pair(calc, open, close);

    }

    solve_calculation_range(calc, calc->head, calc->tail);

}

void solve_calculation_bracket_pair(struct doublist *calc, struct node *open, struct node *close) {
    
    struct node *start = open->next;
    struct node *end = close->prev;
    node_free(calc, open);
    node_free(calc, close);

    if (start != end) {
        solve_calculation_range(calc, start, end);
    }

}

void solve_calculation_range(struct doublist *calc, struct node *start, struct node *end) {

    struct node *afterend = end->next;
    end->next = NULL;

    struct node *n;
    while ((n = node_find(calc, start, "/", IS_OPERATOR, 1, FORWARD)) != NULL) {
        solve_operation(calc, &start, n, &end);
    }
    while ((n = node_find(calc, start, "*", IS_OPERATOR, 1, FORWARD)) != NULL) {
        solve_operation(calc, &start, n, &end);
    }
    while ((n = node_find(calc, start, NULL, IS_OPERATOR, 0, FORWARD)) != NULL) {
        solve_operation(calc, &start, n, &end);
    }

    end->next = afterend;

}

void solve_operation(struct doublist *calc, struct node **start, struct node *op, struct node **end) {

    if (op->prev == NULL && op->next == NULL) {
        node_modify(calc, op, node_create(strtodpntr("0"), IS_NUMBER), 1, 1);

    } else if (op->prev == NULL) {
        node_modify(calc, op, node_create(strtodpntr("0"), IS_NUMBER), 1, 1);
        node_free(calc, op->next);
        
    } else if (op->next == NULL) {
        node_modify(calc, op, node_create(strtodpntr("0"), IS_NUMBER), 1, 1);
        node_free(calc, op->prev);

    }
    
    double solution;

    switch (((char *)op->value)[0]) {

        case '/':
            solution = ((double *)op->prev->value)[0] / ((double *)op->next->value)[0];
            break;

        case '*':
            solution = ((double *)op->prev->value)[0] * ((double *)op->next->value)[0];
            break;

        case '+':
            solution = ((double *)op->prev->value)[0] + ((double *)op->next->value)[0];
            break;

        case '-':
            solution = ((double *)op->prev->value)[0] - ((double *)op->next->value)[0];
            break;

    }

    if (start[0] == op->prev) {
        start[0] = op;
    }
    if (end[0] == op->next) {
        end[0] = op;
    }

    node_free(calc, op->prev);
    node_free(calc, op->next);
    node_modify(calc, op, node_create(xmemdup(&solution, sizeof(solution)), IS_NUMBER), sizeof(solution), 1);

}


double *strtodpntr(char *str) {


    double tmp = strtof(str, NULL);
    return xmemdup(&tmp, sizeof(tmp));

}

#include <stdio.h>
void print_calculation(struct doublist *list) {

    struct node *n = list->head;
    while (n != NULL) {

        if (n->type == IS_NUMBER)
            printf("%f\n", ((double *)n->value)[0]);
        else
            printf("%c\n", ((char *)n->value)[0]);

        n = n->next;
    }

}


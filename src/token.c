#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "xmalloc.h"


/* All functions declared here are only helper functions and are never accessible outside */
/* Append a new node a calculation */
void calculation_append_node(calcualtion *calc, void *item, unsigned int type);
/* Solves the calculation between two brackets */
void solve_calculation_bracket_pair(calcualtion *calc, node *open, node *close);
/* Solves multiple calculations and leaves a node with the result */
void solve_calculation_range(calcualtion *calc, node *start, node *end);
/* Solves a division of two nodes */
void solve_division(calcualtion *calc, node *num1, node *num2);
/* Solves a multiplication of two nodes */
void solve_multiplication(calcualtion *calc, node *num1, node *num2);
/* Solves a addition / substraction of two nodes */
void solve_addition(calcualtion *calc, node *num1, node *num2);
/* Finds a node that is is the given direction of start */
node *find_node(node *start, void *item, int type, int direction);
/* Converts a string to a double pointer */
double *strtofpntr(char *str);
/* Frees a node and sets the pointers */
void free_node(calcualtion *calc, node *n);

/* Functions for debugging */
/* Prints a calculation */
void print_calculator(calcualtion *calc);


/* Create an empty calculation without nodes */
calcualtion *create_calculation() {

    calcualtion *calc = xmalloc(sizeof(calcualtion));
    calc->result = 0.0;
    calc->length = 0;

    calc->first = NULL;
    calc->last = NULL;

    return calc;

}


/* Formats the text and removes unneeded characters */
char *format_text(char *text) {

    int tlen = strlen(text);

    char* format_text = xmalloc(sizeof(char) * tlen + 1);
    int ftlen = 0;

    int i;
    for (i = 0; i < tlen; ++i) {
        /* Only valid chars will be added to the new format_text */
        if (strchr(VALID_CHARS, text[i])) {
            format_text[ftlen++] = text[i];
        }
    }

    /* Terminate the new string and reallocate to only use required memory */
    format_text[ftlen] = '\0';
    format_text = xrealloc(format_text, strlen(format_text) + 1);

    return format_text;

}

/* Creates a new node and appends it to a previously allocated calculation */
void calculation_append_node(calcualtion *calc, void *item, unsigned int type) {

    /* Create node and increment calculator length */
    node *n = xmalloc(sizeof(node));
    n->item = item;
    n->type = type;
    /* next is always NULL because new node is always at the end */
    n->next = NULL;
    ++calc->length;

    /* If the node is the first and only in the calculation */
    if (calc->first == NULL) {

        calc->first = n;
        calc->last = n;
        n->prev = NULL;

    } else {

        /* Appends the node to the calculation and set the pointer from
         * the previous node to the new and new to old */
        n->prev = calc->last;
        calc->last->next = n;
        calc->last = n;

    }

}

/* Reads format_text and converts is to the calculation node structure */
void init_calculation(calcualtion *calc, char *format_text) {

    char *op;

    /* Jump to every operator on the string */
    while ((op = strpbrk(format_text, VALID_OPERATORS)) != NULL) {

        /* len = distance from one operator to another or in other words
         * the length of the number */
        int len = op - format_text;

        /* If an operator is at the beginning of the calculation */
        if (len == 0) {
            calculation_append_node(calc, strndup(format_text, 1), IS_OPERATOR);
            ++format_text;
        } else {
            /* The next part of the string is a number */
            calculation_append_node(calc, strtofpntr(format_text), IS_NUMBER);
            format_text += len;
        }
    }

    /* Append the last number if one exists after the last operator */
    if (format_text[0] != '\0') {
        calculation_append_node(calc, strtofpntr(format_text), IS_NUMBER);
    }

}

/* Takes care of all signs of the numbers */
void format_calculation(calcualtion *calc) {

    node *n = calc->first;
    node *x;

    /* Immagine the calculation is
     * 3-4 then there will be only 2 numbers
     * 3 and -4 and therefore the program only has to add those numbers.
     *
     * Same goes for the addition.
     * 2+6 are to numbers 2 and 6 and only need addition.
     *
     * This function merges the operator with the next number */

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

/* Solves a calculation and handles brackets first */
void solve_calculation(calcualtion *calc) {

    node *open;
    node *close;

    /* Locate the first closing bracket and then go back the first opening bracken.
     * This ensures that the two brackets found are in the right order. */
    while ((close = find_node(calc->first, ")", IS_OPERATOR, FORWARD)) != NULL && (open = find_node(close, "(", IS_OPERATOR, BACKWARDS)) != NULL) {
        solve_calculation_bracket_pair(calc, open, close);
    }

    /* After all brackets are calculated only the last nodes need to be calculated */
    solve_calculation_range(calc, calc->first, calc->last);
    /* solve_calculation_range leaves one node with the result */
    calc->result = ((double *) calc->first->item)[0];

}

/* Solves a pair of brackets */
void solve_calculation_bracket_pair(calcualtion *calc, node *open, node *close) {
/* node *open is the pointer to the node whose item is the '(' */
/* node *close is the pointer to the node whose item is the ')' */

    /* Free both brackets */
    node *start = open->next;
    node *end = close->prev;
    free_node(calc, open);
    free_node(calc, close);
    /* Only start calculating if there are more that 2 numbers in the brackets */
    if (start != end) {
        solve_calculation_range(calc, start, end);
    }
    /* Format calculation again because the solving of the brackets could have revealed new signs */
    format_calculation(calc);

}

void solve_calculation_range(calcualtion *calc, node *start, node *end) {
/* node *start contains the pointer to the first number of the calculation range */
/* node *end contains the pointer to the last number of the calculation range */

    /* Create a new sub-calculation */
    calcualtion *c = create_calculation();
    c->first = start;
    c->last = end;
    /* end emporarily set as the last number is the calculation */
    node *afterend = end->next;
    end->next = NULL;

    /* Only used when main-calculation is the same as sub-calculation */
    int syncfirsts = 0;
    int synclasts = 0;

    if (c->first == calc->first)
        syncfirsts = 1;
    if (c->last == calc->last)
        synclasts = 1;

    /* Find the first division in the sub-calculation */
    node *n;
    while ((n = find_node(c->first, "/", IS_OPERATOR, FORWARD)) != NULL) {
        solve_division(c, n->prev, n->next);
    }

    /* Find the first multiplication of the sub-calculation */
    while ((n = find_node(c->first, "*", IS_OPERATOR, FORWARD)) != NULL) {
        solve_multiplication(c, n->prev, n->next);

    }

    /* Checks for two number after each another and add them
     * more information why no substraction is presend is in format_calculation */
    n = c->first;
    while (n != NULL && n->type == IS_NUMBER && n->next != NULL && n->next->type == IS_NUMBER) {

        solve_addition(c, n, n->next);
    }

    /* Sets main-calculations pointers to the sub-calculations because those may have changed */
    if (syncfirsts)
        calc->first = c->first;
    if (synclasts)
        calc->last = c->last;

    /* Stop end to be pretending to be the last node */
    c->last->next = afterend;
    if (afterend != NULL) {
        c->last->next->prev = c->last;
    }
    xfree(c);

}

/* Solves a division of two numbers */
void solve_division(calcualtion *calc, node *num1, node *num2) {
/* node *num1 is the number left of the division symbol */
/* node *num2 is the number right of the division symbol */

    double product = ((double *) num1->item)[0] / ((double *) num2->item)[0];

    /* Replace operator with the calculated number */
    node *operator = num1->next;
    xfree(operator->item);
    operator->item = xmalloc(sizeof(double));
    memmove(operator->item, &product, sizeof(double));
    operator->type = IS_NUMBER;

    /* Frees both unneded numbers */
    free_node(calc, num1);
    free_node(calc, num2);

}

void solve_multiplication(calcualtion *calc, node *num1, node *num2) {
/* node *num1 is the number left of the multiplication symbol */
/* node *num2 is the number right of the multiplication symbol */

    double product = ((double *) num1->item)[0] * ((double *) num2->item)[0];

    /* Replace operator with the calculated number */
    node *operator = num1->next;
    xfree(operator->item);
    operator->item = xmalloc(sizeof(double));
    memmove(operator->item, &product, sizeof(double));
    operator->type = IS_NUMBER;

    /* Frees both unneeded numbers */
    free_node(calc, num1);
    free_node(calc, num2);

}

void solve_addition(calcualtion *calc, node *num1, node *num2) {
/* node *num1 is the number left of the addition symbol */
/* node *num2 is the number right of the addition symbol */

    double sum = ((double *) num1->item)[0] + ((double *) num2->item)[0];

    /* Replace first number (num1) with the calculated number */
    xfree(num1->item);
    num1->item = xmalloc(sizeof(double));
    memmove(num1->item, &sum, sizeof(double));
    num1->type = IS_NUMBER;

    /* Frees unneeded number */
    free_node(calc, num2);

}

node *find_node(node *start, void *item, int type, int direction) {

    node *n = start;

    if (direction == FORWARD) {

        while (n != NULL) {
            if (memcmp(n->item, item, type == IS_NUMBER ? sizeof(double) : 1) == 0) {
                return n;
            }
            n = n->next;
        }
        return NULL;

    } else {
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

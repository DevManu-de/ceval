#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calculation.h"
#include "doublist.h"
#include "xmemtools.h"


/* Solves the calculation between two brackets */
void solve_calculation_bracket_pair(struct doublist *calc, struct node *open, struct node *close);
/* Solves multiple calculations and leaves a node with the result */
void solve_calculation_range(struct doublist *calc, struct node *start, struct node *end);
/* Solves a division of two nodes */
void solve_division(struct doublist *calc, struct node *num1, struct node *num2);
/* Solves a multiplication of two nodes */
void solve_multiplication(struct doublist *calc, struct node *num1, struct node *num2);
/* Solves a addition / substraction of two nodes */
void solve_addition(struct doublist *calc, struct node *num1, struct node *num2);
/* Converts a string to a double pointer */
double *strtofpntr(char *str);

/* Functions for debugging */
/* Prints a calculation */
void print_calculation(struct doublist *calc);


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

/* Reads format_text and converts is to the calculation node structure */
void init_calculation(struct doublist *calc, char *format_text) {

    char *op;

    /* Jump to every operator on the string */
    while ((op = strpbrk(format_text, VALID_OPERATORS)) != NULL) {

        /* len = distance from one operator to another or in other words
         * the length of the number */
        int len = op - format_text;

        /* If an operator is at the beginning of the calculation */
        if (len == 0) {
            node_insert_after(calc, NULL, node_create(strndup(format_text, 1), IS_OPERATOR));
            //calculation_append_node(calc, strndup(format_text, 1), IS_OPERATOR);
            ++format_text;
        } else {
            /* The next part of the string is a number */
            node_insert_after(calc, NULL, node_create(strtofpntr(format_text), IS_NUMBER));
            //calculation_append_node(calc, strtofpntr(format_text), IS_NUMBER);
            format_text += len;
        }
    }

    /* Append the last number if one exists after the last operator */
    if (format_text[0] != '\0') {
        node_insert_after(calc, NULL, node_create(strtofpntr(format_text), IS_NUMBER));
        //calculation_append_node(calc, strtofpntr(format_text), IS_NUMBER);
    }

}

/* Takes care of all signs of the numbers */
void format_calculation(struct doublist *calc) {

    struct node *n = calc->head;
    struct node *x;

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
        if (n->type == IS_OPERATOR && ((char *) n->value)[0] == '-') {
            if (n->next != NULL && n->next->type == IS_NUMBER) {
                ((double *) n->next->value)[0] *= -1.0;
                node_free(calc, n);

            }

        } else if (n->type == IS_OPERATOR && ((char *) n->value)[0] == '+') {
            if (n->next->type == IS_NUMBER) {
                node_free(calc, n);
            }
        }
        n = x;
    }

}

/* Solves a calculation and handles brackets first */
void solve_calculation(struct doublist *calc) {

    struct node *open;
    struct node *close;

    /* Locate the first closing bracket and then go back the first opening bracken.
     * This ensures that the two brackets found are in the right order. */
    //while ((close = find_node(calc->first, ")", IS_OPERATOR, FORWARD)) != NULL && (open = find_node(close, "(", IS_OPERATOR, BACKWARD)) != NULL) {
    //    solve_calculation_bracket_pair(calc, open, close);
    //}
    while ((close = node_find(calc, calc->head, ")", IS_OPERATOR, 1, FORWARD)) != NULL && (open = node_find(calc, close, "(", IS_OPERATOR, 1, BACKWARD)) != NULL) {
        solve_calculation_bracket_pair(calc, open, close);
    }

    /* After all brackets are calculated only the last nodes need to be calculated */
    solve_calculation_range(calc, calc->head, calc->tail);
    /* solve_calculation_range leaves one node with the result */
    //calc->result = ((double *) calc->head->vaue)[0];

}

/* Solves a pair of brackets */
void solve_calculation_bracket_pair(struct doublist *calc, struct node *open, struct node *close) {
/* node *open is the pointer to the node whose item is the '(' */
/* node *close is the pointer to the node whose item is the ')' */

    /* Free both brackets */
    struct node *start = open->next;
    struct node *end = close->prev;
    node_free(calc, open);
    node_free(calc, close);
    /* Only start calculating if there are more that 2 numbers in the brackets */
    if (start != end) {
        solve_calculation_range(calc, start, end);
    }
    /* Format calculation again because the solving of the brackets could have revealed new signs */
    format_calculation(calc);

}

void solve_calculation_range(struct doublist *calc, struct node *start, struct node *end) {
/* node *start contains the pointer to the first number of the calculation range */
/* node *end contains the pointer to the last number of the calculation range */

    /* Create a new sub-calculation */
    struct doublist *c = doublist_create();
    c->head = start;
    c->tail = end;
    /* end emporarily set as the last number is the calculation */
    struct node *afterend = end->next;
    end->next = NULL;

    /* Only used when main-calculation is the same as sub-calculation */
    int syncfirsts = 0;
    int synclasts = 0;

    if (c->head == calc->head)
        syncfirsts = 1;
    if (c->tail == calc->tail)
        synclasts = 1;

    /* Find the first division in the sub-calculation */
    struct node *n;
    //while ((n = find_node(c->first, "/", IS_OPERATOR, FORWARD)) != NULL) {
    //    solve_division(c, n->prev, n->next);
    //}
    while ((n = node_find(c, c->head, "/", IS_OPERATOR, 1, FORWARD)) != NULL) {
        solve_division(c, n->prev, n->next);
    }

    /* Find the first multiplication of the sub-calculation */
    //while ((n = find_node(c->first, "*", IS_OPERATOR, FORWARD)) != NULL) {
    //    solve_multiplication(c, n->prev, n->next);
    //}
    while ((n = node_find(c, c->head, "*", IS_OPERATOR, 1, FORWARD)) != NULL) {
        solve_multiplication(c, n->prev, n->next);
    }

    /* Checks for two number after each another and add them
     * more information why no substraction is presend is in format_calculation */
    n = c->head;
    while (n != NULL && n->type == IS_NUMBER && n->next != NULL && n->next->type == IS_NUMBER) {

        solve_addition(c, n, n->next);
    }

    /* Sets main-calculations pointers to the sub-calculations because those may have changed */
    if (syncfirsts)
        calc->head = c->head;
    if (synclasts)
        calc->tail = c->tail;

    /* Stop end to be pretending to be the last node */
    c->tail->next = afterend;
    if (afterend != NULL) {
        c->tail->next->prev = c->tail;
    }
    xfree(c);

}

/* Solves a division of two numbers */
void solve_division(struct doublist *calc, struct node *num1, struct node *num2) {
/* node *num1 is the number left of the division symbol */
/* node *num2 is the number right of the division symbol */

    double product = ((double *) num1->value)[0] / ((double *) num2->value)[0];

    /* Replace operator with the calculated number */
    struct node *operator = num1->next;
    //replace_node(operator, &product, IS_NUMBER);
    node_modify(calc, operator, node_create(xmemdup(&product, sizeof(product)), IS_NUMBER), sizeof(product), 1);

    /* Frees both unneded numbers */
    node_free(calc, num1);
    node_free(calc, num2);

}

/* Solves a multiplication of two numbers */
void solve_multiplication(struct doublist *calc, struct node *num1, struct node *num2) {
/* node *num1 is the number left of the multiplication symbol */
/* node *num2 is the number right of the multiplication symbol */

    double product = ((double *) num1->value)[0] * ((double *) num2->value)[0];

    /* Replace operator with the calculated number */
    struct node *operator = num1->next;
    //replace_node(operator, &product, IS_NUMBER);
    node_modify(calc, operator, node_create(xmemdup(&product, sizeof(product)), IS_NUMBER), sizeof(product), 1);

    /* Frees both unneeded numbers */
    node_free(calc, num1);
    node_free(calc, num2);

}

/* Solves a addition / division of two numbers */
void solve_addition(struct doublist *calc, struct node *num1, struct node *num2) {
/* node *num1 is the number left of the addition symbol */
/* node *num2 is the number right of the addition symbol */

    double sum = ((double *) num1->value)[0] + ((double *) num2->value)[0];

    /* Replace first number (num1) with the calculated number */
    node_modify(calc, num1, node_create(xmemdup(&sum, sizeof(sum)), IS_NUMBER), sizeof(sum), 1);
    //replace_node(num1, &sum, IS_NUMBER);

    /* Frees unneeded number */
    node_free(calc, num2);

}


/* Converts a string to a double pointer */
double *strtofpntr(char *str) {

    double f = strtof(str, NULL);
    double *fpntr = xmalloc(sizeof(double));
    fpntr = memmove(fpntr, &f, sizeof(double));

    return fpntr;
}

/* Only used for debugging */
/* Prints an entire calculation */
void print_calculation(struct doublist *calc) {

    puts("\n\n\n");

    struct node *n = calc->head;
    while (n != NULL) {
        if (n->type == IS_NUMBER)
            printf("%f\n", ((double *) n->value)[0]);
        else
            puts(n->value);
        n = n->next;
    }

    puts("\n\n\n");
}

#ifndef _TOKEN_H
#define _TOKEN_H

#define VALID_CHARS "0123456789+-*/()."
#define VALID_NUMBERS "0123456789"
#define VALID_OPERATORS "+-*/()"

typedef struct _node {

    unsigned int type : 2;
    void *item;
    struct _node *next;
    struct _node *prev;

} node;

typedef struct _calculation {

    int length;
    struct _node *first;
    struct _node *last;

} calcualtion;

calcualtion *create_calculation(int size);
char *format_text(char *text);
void init_calculation(calcualtion *calc, char *format_text);
void free_calculation(calcualtion *calc);

#endif

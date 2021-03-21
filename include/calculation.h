#ifndef _CALCULATION_H
#define _CALCULATION_H

#include "doublist.h"

#define VALID_CHARS "0123456789+-*/()."
#define VALID_NUMBERS "0123456789"
#define VALID_OPERATORS "+-*/()"

#define IS_NUMBER 1
#define IS_OPERATOR 2
#define IS_NULL 0

char *format_text(char *text);
void init_calculation(struct doublist *calc, char *format_text);
void format_calculation(struct doublist *calc);
void solve_calculation(struct doublist *calc);

#endif

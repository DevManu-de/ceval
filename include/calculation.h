#ifndef _CALCULATION_H
#define _CALCULATION_H

#include "doublist.h"

#define VALID_CHARS "0123456789+-*/()."
#define VALID_NUMBERS "0123456789"
#define VALID_OPERATORS "+-*/()"

char *format_text(char *text);
void init_calculation(struct doublist *calc, char *format_text);
void format_calculation(struct doublist *calc);
void solve_calculation(struct doublist *calc);

#endif

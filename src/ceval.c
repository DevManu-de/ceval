#include "ceval.h"

#include "doublist.h"

#define VALID_CHARS "0123456789+-*/()."
#define VALID_NUMBERS "0123456789"
#define VALID_OPERATORS "+-*/()"

#define IS_NUMBER 1
#define IS_OPERATOR 2
#define IS_NULL 0

typedef struct doublist calcualtion;

calcualtion *create_calculation();
char *format_text(char *text);
void init_calculation(calcualtion *calc, char *format_text);
void format_calculation(calcualtion *calc);
void solve_calculation(calcualtion *calc);
void free_calculation(calcualtion *calc);

double ceval(char *_calc_) {

    return 0;

}


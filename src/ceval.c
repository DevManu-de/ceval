
#include <stdio.h>
#include <string.h>


#include "ceval.h"
#include "token.h"
#include "xmalloc.h"

double ceval(char *text) {

    char *formatted_text = format_text(text);
    calcualtion *clc = create_calculation();
    init_calculation(clc, formatted_text);
    format_calculation(clc);
    solve_calculation(clc);

    double result = clc->result;

    xfree(formatted_text);
    free_calculation(clc);

    return result == 0 ? 0 : result;

}

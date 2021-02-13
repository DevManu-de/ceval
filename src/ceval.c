
#include <stdio.h>
#include <string.h>


#include "ceval.h"
#include "calculation.h"
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

    /* Sometimes -0 is returnd and this changes it to 0 */
    return result == 0 ? 0 : result;

}

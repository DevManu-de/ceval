
#include <stdio.h>
#include <string.h>


#include "ceval.h"
#include "calculation.h"
#include "xmalloc.h"

double ceval(char *text) {

    /* First format text to simplifly for parsing later */
    char *formatted_text = format_text(text);
    /* Creates an empty calculation */
    calcualtion *clc = create_calculation();
    /* Convert text into a doubly linked list */
    init_calculation(clc, formatted_text);
    /* Handle signs of numbers */
    format_calculation(clc);
    /* Solve the calculation and saves result in result */
    solve_calculation(clc);

    double result = clc->result;

    /* Free everything */
    xfree(formatted_text);
    free_calculation(clc);

    /* Sometimes -0 is returnd and this changes it to 0 */
    return result == 0 ? 0 : result;

}

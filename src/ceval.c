#include <stdio.h>
#include <string.h>


#include "ceval.h"
#include "calculation.h"
#include "doublist.h"
#include "xmemtools.h"

double ceval(char *text) {

    /* First format text to simplifly for parsing later */
    char *formatted_text = format_text(text);
    /* Creates an empty calculation */
    struct doublist *clc = doublist_create();
    /* Convert text into a doubly linked list */
    init_calculation(clc, formatted_text);
    /* Handle signs of numbers */
    format_calculation(clc);
    /* Solve the calculation and saves result in result */
    solve_calculation(clc);

    double result = ((double *)clc->head->value)[0];

    /* Free everything */
    xfree(formatted_text);
    doublist_free(clc);

    /* Sometimes -0 is returnd and this changes it to 0 */
    return result == 0 ? 0 : result;

}

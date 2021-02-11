
#include <stdio.h>
#include <string.h>


#include "ceval.h"
#include "token.h"
#include "xmalloc.h"

double ceval(char *text) {

    char *formatted_text = format_text(text);
    calcualtion *clc = create_calculation();
    init_calculation(clc, formatted_text);
    prepare_calculation(clc);
    solve_calculation(clc);

    node *n = clc->first;
    while (n != NULL && n->type != IS_NULL) {
        if (n->type == IS_NUMBER)
            printf("%f\n", *(double *) n->item);
        else
            puts(n->item);
        n = n->next;
    }

    double result = clc->result;

    xfree(formatted_text);
    free_calculation(clc);

    return result;

}

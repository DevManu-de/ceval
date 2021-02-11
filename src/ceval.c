
#include <stdio.h>
#include <string.h>


#include "ceval.h"
#include "token.h"
#include "xmalloc.h"

long double ceval(char *text) {

    char *formatted_text = format_text(text);
    calcualtion *clc = create_calculation(strlen(text));
    init_calculation(clc, formatted_text);

    node *n = clc->first;
    while (n != NULL && n->type != 0) {
        if (n->type == 1)
            printf("%f\n", *(double *) n->item);
        else
            puts(n->item);
        n = n->next;
    }

    xfree(formatted_text);
    free_calculation(clc);

    return 0.0;

}

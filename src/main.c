
#include <stdio.h>

#include "ceval.h"
#include "token.h"
#include "xmalloc.h"


int main() {

    //printf("%f\n", ceval("2+3/553*(-99+7)"));
    //printf("%f\n", ceval("1+(2-2)*3"));
    //printf("%f\n", ceval("2*7/1.5 * 1.4"));

    //printf("%f\n", ceval("(2-3)*(2-2)"));
    printf("%f\n", ceval("1+(3-4+(8-7))"));

    return 0;
}

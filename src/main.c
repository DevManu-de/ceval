
#include <stdio.h>

#include "ceval.h"
#include "token.h"
#include "xmalloc.h"


int main() {

    //printf("%f\n", ceval("2+3/553*(-99+7)"));
    printf("%f\n", ceval("1+(2-2)*3"));
    printf("%f\n", ceval("2*7/1.5 * 1.4"));
    //char in[100];
    //fgets(in, 99, stdin);

    printf("%f\n", ceval("(3−3)(3−2)"));

    return 0;
}

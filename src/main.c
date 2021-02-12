
#include <stdio.h>

#include "ceval.h"
#include "token.h"
#include "xmalloc.h"


int main() {

    //printf("%f\n", ceval("(-(1*2-(-3)/553) -99)+ 7"));
    printf("%f\n", ceval("2*7/2"));

    return 0;
}

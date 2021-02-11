
#include <stdio.h>

#include "ceval.h"
#include "token.h"
#include "xmalloc.h"


int main() {

    printf("%Lf\n", ceval("-1*2+3"));

    return 0;
}

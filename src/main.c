
#include <stdio.h>

#include "ceval.h"
#include "token.h"
#include "xmalloc.h"


int main() {

    char *a = "12345ab/-0x";
    a = format_text(a);
    puts(a);
    xfree(a);

    return 0;
}

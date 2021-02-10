
#include <stdio.h>

#include "ceval.h"
#include "token.h"



int main() {

    char *a = "12345ab/-0x";
    puts(format_text(a));

    return 0;
}

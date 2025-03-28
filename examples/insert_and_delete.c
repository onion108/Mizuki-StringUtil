#include "mizukistr.h"
#include <stdio.h>

int main(void) {
    MZOwnedString *str = MZOwnedString_from_cstr("     Hello? World!  ");


    // 1: insert string
    MZOwnedString_replace_at(str, (MZStringView) { .data = str->data+10, .len = 0}, MIZUKI_CVIEW(","));
    printf("Inserted: " MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));


    // 2: delete string
    MZOwnedString_replace_at(str, (MZStringView) { .data = str->data+11, .len = 1}, MIZUKI_CVIEW(""));
    printf("Inserted: " MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));
}


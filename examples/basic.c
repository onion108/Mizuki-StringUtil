#include "mizukistr.h"
#include <stdio.h>

int main(void) {
    // Create a string.
    MZOwnedString *str = MZOwnedString_from_cstr("Hello, World! ");
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    // Append things to a string.
    MZOwnedString_push(str, ',');
    MZOwnedString_push_cstr(str, "but something append");
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    // Slice a piece from string.
    MZStringView sv = MZOwnedString_slicesl(str, 0, 5);
    MZStringView sv2 = MZOwnedString_slicesl(str, 7, 5);
    printf("sv = " MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARGS(sv));
    printf("sv2 = " MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARGS(sv2));

    // Replace slice.
    printf("=============REPLACE TESTS============\n");
    MZOwnedString_replace_at(str, sv2, MZStringView_from_cstr("Urmom"));
    sv2 = MZOwnedString_slicesl(str, 7, 5);
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    MZOwnedString_replace_at(str, sv2, MZStringView_from_cstr("abc"));
    sv2 = MZOwnedString_slicesl(str, 7, 3);
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    MZOwnedString_replace_at(str, sv2, MZStringView_from_cstr("whatever it is but it must be a long string to test if reallocation works as expected"));
    sv2 = MZOwnedString_slicesl(str, 7, 85);
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    MZOwnedString_replace_at(str, sv2, MZStringView_from_cstr(""));
    sv2 = MZOwnedString_slicesl(str, 7, 0);
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    MZOwnedString_replace_at(str, sv2, MZStringView_from_cstr("I'm back"));
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    MZOwnedString_delete(str);
}


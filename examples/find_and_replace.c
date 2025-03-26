#include "mizukistr.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    MZOwnedString *str = MZOwnedString_from_cstr("Here is some ## that need to be replaced with $$. ");

    auto m = MZStringView_find(MZStringView_from_str(str), MZStringView_from_cstr("##"), nullptr);
    assert(!MZStringView_is_invalid(m));
    MZOwnedString_replace_at(str, m, MZStringView_from_cstr("text"));
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    auto n = MZStringView_find(MZStringView_from_str(str), MZStringView_from_cstr("$$"), nullptr);
    assert(!MZStringView_is_invalid(n));
    MZOwnedString_replace_at(str, n, MZStringView_from_cstr("another text"));
    printf(MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARG(str));

    MZOwnedString_delete(str);
}

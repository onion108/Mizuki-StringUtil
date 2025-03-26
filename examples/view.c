#include "mizukistr.h"
#include <stdio.h>

int main(void) {
    MZStringView sv = MZStringView_from_cstr("Some C-style string");
    printf("Original: " MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARGS(sv));
    char start = MZStringView_chop_char(&sv);
    char end = MZStringView_rchop_char(&sv);
    printf("Start char: %c\n", start);
    printf("End char: %c\n", end);
    printf("View after chopping: " MIZUKI_FMT_SPEC "\n", MIZUKI_FMTARGS(sv));

    sv = MZStringView_from_cstr("asjpi poicmopiqejnoijfmoiwjdfnoiwjmijmkkdmlkjmalskfmaslkm");
    printf("Reversed string: ");
    char ch;
    while ((ch = MZStringView_rchop_char(&sv))) {
        putchar(ch);
    }
    putchar('\n');
}


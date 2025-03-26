#include "mizukistr.h"
#include <string.h>
#include <stdlib.h>
static MZStringView trivial_find(MZStringView sv, MZStringView pat, MZStringView *remain) {
    const char *orig = sv.data+sv.len;
    while (sv.len >= pat.len) {
        MZStringView head = (MZStringView) {
            .data = sv.data,
            .len = pat.len,
        };
        if (MZStringView_equal(head, pat)) {
            if (remain) {
                remain->data = head.data+head.len;
                remain->len = orig-head.data-head.len;
            }
            return head;
        }
        MZStringView_chop_char(&sv);
    }
    return MZStringView_invalid();
}
MZStringView MZStringView_from_cstr(const char *str) {
    return (MZStringView) {
        .data = str,
        .len = strlen(str),
    };
}

MZStringView MZStringView_from_str(MZOwnedString *str) {
    return (MZStringView) {
        .data = str->data,
        .len = str->len,
    };
}

MZStringView MZStringView_invalid() {
    return (MZStringView) {
        .data = nullptr,
        .len = 0,
    };
}

MZStringView MZStringView_from_addr(const char *addr, size_t len) {
    return (MZStringView) {
        .data = addr,
        .len = 0,
    };
}

bool MZStringView_is_empty(MZStringView sv) {
    return sv.data ? sv.len == 0 : false;
}

bool MZStringView_is_invalid(MZStringView sv) {
    return !sv.data;
}

char MZStringView_chop_char(MZStringView *sv) {
    if (!sv || MZStringView_is_empty(*sv)) return '\0';
    return *(sv->len--, sv->data++);
}

char MZStringView_rchop_char(MZStringView *sv) {
    if (!sv || MZStringView_is_empty(*sv)) return '\0';
    return sv->data[(sv->len--)-1];
}

void MZStringView_drop_while(MZStringView *sv, bool (*pred)(char)) {
    if (!sv || MZStringView_is_empty(*sv)) return;
    
    while (sv->len && pred(sv->data[0])) {
        MZStringView_chop_char(sv);
    }
}

void MZStringView_rdrop_while(MZStringView *sv, bool (*pred)(char)) {
    if (!sv || MZStringView_is_empty(*sv)) return;
    
    while (sv->len && pred(sv->data[sv->len-1])) {
        MZStringView_rchop_char(sv);
    }
}

bool MZStringView_equal(MZStringView sv, MZStringView rhs) {
    if (!sv.data || !rhs.data) return false;
    return sv.len == rhs.len ? memcmp(sv.data, rhs.data, sv.len) : false;
}

MZStringView MZStringView_find(MZStringView sv, MZStringView pat, MZStringView *remain) {
    if (MZStringView_is_empty(sv)) {
        if (remain) remain->data = sv.data, remain->len = sv.len;
        return MZStringView_invalid();
    }

    if (MZStringView_is_empty(pat)) {
        if (remain) *remain = sv;
        return (MZStringView) { .data = sv.data, .len = 0 };
    }

    if (pat.len > sv.len) {
        if (remain) *remain = sv;
        return MZStringView_invalid();
    }

    size_t *lps = malloc(pat.len*sizeof(size_t));
    if (!lps) return trivial_find(sv, pat, remain);

    {
        int len = 0;
        lps[0] = 0;
        {
            int i = 1;
            while (i < pat.len) {
                if (pat.data[i] == pat.data[len]) {
                    lps[i++] = ++len;
                } else {
                    if (len) {
                        len = lps[len-1];
                    } else {
                        lps[i++] = 0;
                    }
                }
            }
        }
    }

    int i = 0;
    int j = 0;

    while (i < sv.len) {
        if (sv.data[i] == pat.data[j]) {
            i++, j++;

            if (j == pat.len) {
                size_t idx = i-j;
                if (remain) {
                    remain->data = sv.data+idx+pat.len;
                    remain->len = sv.len-idx-pat.len;
                }
                return (MZStringView) {
                    .data = sv.data+idx,
                    .len = pat.len,
                };
                // j = lps[j-1];
            }
        } else {
            if (j) j = lps[j-1];
            else i++;
        }
    }
    free(lps);

    if (remain) *remain = sv;
    return MZStringView_invalid();
    
}


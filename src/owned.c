#include "mizukistr.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>

static bool try_realloc(MZOwnedString *str) {
    assert(str != nullptr);
    assert(str->data != nullptr);
    size_t new_cap;
    if (str->cap <= 1024) {
        new_cap = str->cap * 2;
    } else {
        new_cap = str->cap + str->cap / 2;
    }
    char *new_addr = realloc(str->data, new_cap);
    if (!new_addr) return false;
    str->cap = new_cap;
    str->data = new_addr;
    return true;
}

MZOwnedString *MZOwnedString_new() {
    MZOwnedString *result = malloc(sizeof(MZOwnedString));
    if (!result) return nullptr;

    result->cap = 16;
    result->data = malloc(16);
    if (!result->data) {
        free(result);
        return nullptr;
    }
    result->len = 0;
    return result;
}

MZOwnedString *MZOwnedString_from_cstr(const char *str) {
    MZOwnedString *result = malloc(sizeof(MZOwnedString));
    if (!result) return nullptr;

    size_t cstr_len = strlen(str);
    if (cstr_len % 8 == 0) {
        result->cap = cstr_len;
    } else {
        result->cap = cstr_len / 8 * 8 + 8;
    }

    result->data = malloc(result->cap);
    if (!result->data) {
        free(result);
        return nullptr;
    }
    memcpy(result->data, str, cstr_len);

    result->len = cstr_len;
    return result;
}

MZOwnedString *MZOwnedString_from_view(MZStringView view) {
    MZOwnedString *result = malloc(sizeof(MZOwnedString));
    if (!result) return nullptr;

    size_t length = view.len;
    if (length % 8 == 0) {
        result->cap = length;
    } else {
        result->cap = length / 8 * 8 + 8;
    }

    result->data = malloc(result->cap);
    if (!result->data) {
        free(result);
        return nullptr;
    }
    memcpy(result->data, view.data, length);

    result->len = length;
    return result;
}

MZOwnedString *MZOwnedString_from_sprintf(const char *fmt, ...) {
    MZOwnedString *result = malloc(sizeof(MZOwnedString));
    if (!result) return nullptr;

    va_list args;
    va_start(args, fmt);
    size_t length = vsnprintf(nullptr, 0, fmt, args);
    if (length % 8 == 0) {
        result->cap = length;
    } else {
        result->cap = length / 8 * 8 + 8;
    }

    result->data = malloc(result->cap);
    if (!result->data) {
        free(result);
        return nullptr;
    }
    va_start(args, fmt);
    vsprintf(result->data, fmt, args);
    result->len = length;
    return result;
}

void MZOwnedString_delete(MZOwnedString *str) {
    if (str) {
        if (str->data) free(str->data);
        free(str);
    }
}

MZStringView MZOwnedString_slicesl(MZOwnedString *str, size_t start, size_t length) {
    if (!str || !str->data) return MZStringView_invalid();

    if (start >= str->len || start+length >= str->len) return MZStringView_invalid();

    return (MZStringView) {
        .data = str->data+start,
        .len = length,
    };
}

MZStringView MZOwnedString_slicebe(MZOwnedString *str, size_t begin, size_t end) {
    if (!str || !str->data) return MZStringView_invalid();
    if (begin >= str->len || end >= str->len || begin > end) return MZStringView_invalid();
    return (MZStringView) {
        .data = str->data+begin,
        .len = end-begin,
    };
}

bool MZOwnedString_push(MZOwnedString *str, char c) {
    if (!str || !str->data) return false;
    if (str->len >= str->cap) {
        if (!try_realloc(str)) {
            return false;
        }
    }

    str->data[str->len++] = c;
    return true;
}

bool MZOwnedString_push_sv(MZOwnedString *str, MZStringView sv) {
    if (!str || !str->data) return false;
    while (str->cap < str->len+sv.len) {
        if (!try_realloc(str)) {
            return false;
        }
    }

    memcpy(str->data+str->len, sv.data, sv.len);
    str->len += sv.len;
    return true;
}

bool MZOwnedString_push_string(MZOwnedString *str, MZOwnedString *rhs) {
    return MZOwnedString_push_sv(str, (MZStringView) {
        .data = rhs->data,
        .len = rhs->len,
    });
}

bool MZOwnedString_push_cstr(MZOwnedString *str, const char *rhs) {
    return MZOwnedString_push_sv(str, (MZStringView) {
        .data = rhs,
        .len = strlen(rhs),
    });
}

bool MZOwnedString_replace_at(MZOwnedString *str, MZStringView from, MZStringView to) {
    if (!str) return false;
    if (from.data < str->data || from.data >= str->data+str->len) return false;
    if (from.data+from.len < str->data || from.data+from.len >= str->data+str->len) return false;

    if (from.len == to.len) {
        memmove((char *) from.data, to.data, from.len);
        return true;
    }

    if (from.len > to.len) {
        size_t delta = from.len-to.len;
        memmove((char *) from.data+to.len, from.data+from.len, (str->data+str->len-from.data-from.len));
        memmove((char *) from.data, to.data, to.len);
        str->len -= delta;
        return true;
    }

    // Otherwise needs allocation.
    size_t delta = to.len-from.len;
    size_t offset = from.data-str->data;
    while (str->len+delta > str->cap) {
        if (!try_realloc(str)) return false;
    }
    char *ptr = str->data+offset;
    memmove(ptr+to.len, ptr+from.len, (str->data+str->len-ptr-from.len));
    memcpy(ptr, to.data, to.len);
    str->len += delta;
    return true;
}


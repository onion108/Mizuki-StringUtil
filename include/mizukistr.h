#ifndef ONION27_MIZUKI_STR_H_
#define ONION27_MIZUKI_STR_H_

#include <string.h>
#include <stddef.h>
#include <stdint.h>

#ifndef MIZUKI_STRING_NO_UTIL_MACROS

#ifdef __cplusplus
#define MIZUKI_C_LITERAL(type) type
#else
#define MIZUKI_C_LITERAL(type) (type)
#endif

// Construct string view from c-style string.
#define MIZUKI_CVIEW(cstr) MIZUKI_C_LITERAL(MZStringView) { .data = cstr, .len = strlen(cstr) }
#ifdef MIZUKI_STRING_SHORT_MACRO
#define C_(...) MIZUKI_CVIEW(__VA_ARGS__)
#endif

// Construct string view from string.
#define MIZUKI_SVIEW(str) MIZUKI_C_LITERAL(MZStringView) { .data = str->data, .len = str->len }
#ifdef MIZUKI_STRING_SHORT_MACRO
#define S_(...) MIZUKI_SVIEW(__VA_ARGS__)
#endif

#endif

#ifndef MIZUKI_STRING_NO_ALLOC

// A structure represents an **owned** string.
// We assume the data is allocated on the heap.
typedef struct MZOwnedString_st_ {
    char *data;
    size_t len;
    size_t cap;
} MZOwnedString;

#endif

// A read only string view.
typedef struct MZStringView_st_ {
    const char *data;
    size_t len;
} MZStringView;

// Format specifier used to output a string or string view from this library.
#define MIZUKI_FMT_SPEC "%.*s"

// Wrap your string or string view (in pointer format) in this macro and pass it to printf.
#define MIZUKI_FMTARG(s) (int) ((s)->len), s->data

// Wrap your string or string view (in struct format) in this macro and pass it to printf.
#define MIZUKI_FMTARGS(s) (int) ((s).len), s.data

#if defined(__clang__) || defined(__GNUC__)
#define MZKprivate__FORMAT_ATTRIB_(...) __attribute__ ((format(__VA_ARGS__)))
#else
#define MZKprivate__FORMAT_ATTRIB_(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MIZUKI_STRING_NO_ALLOC

// Allocates an empty string. Returns nullptr on allocation failure.
MZOwnedString *MZOwnedString_new();

// Allocate a string from c string. Returns nullptr on allocation failure.
MZOwnedString *MZOwnedString_from_cstr(const char *str);

// Allocate a string from string view. Returns nullptr on allocation failure.
MZOwnedString *MZOwnedString_from_view(MZStringView view);

// Allocate a string from format string. Returns nullptr on allocation failure.
MZOwnedString *MZOwnedString_from_sprintf(const char *fmt, ...) MZKprivate__FORMAT_ATTRIB_(printf, 1, 2) ;

// Deallocate a string.
void MZOwnedString_delete(MZOwnedString *str);

// Push a character to a string.
// Returns false if allocation error occured.
bool MZOwnedString_push(MZOwnedString *str, char c);

// Push a string view to a string.
// Returns false if allocation error occured.
bool MZOwnedString_push_sv(MZOwnedString *str, MZStringView sv);

// Push a string to a string.
// Returns false if allocation error occured.
bool MZOwnedString_push_string(MZOwnedString *str, MZOwnedString *rhs);

// Push a C-style string to a string.
// Returns false if allocation error occured.
bool MZOwnedString_push_cstr(MZOwnedString *str, const char *rhs);

// Slice substr from the string using start and length.
MZStringView MZOwnedString_slicesl(MZOwnedString *str, size_t start, size_t length);

// Slice substr from the string using begin index and end index.
MZStringView MZOwnedString_slicebe(MZOwnedString *str, size_t begin, size_t end);

// Replace the given string view `from` to another string view `to`. Notice the `from` must belong to the string.
// Returns false if allocation error occured or x is not belong to the string.
// Please notice `from` will be invalid after replacement.
bool MZOwnedString_replace_at(MZOwnedString *str, MZStringView from, MZStringView to);

// Borrow owned string as C-style string.
// NOTICE! You are always supposed to call this function to get the correct C-style string pointer instead of using data member directly, because data might not be zero-terminated before calling to this function.
char *MZOwnedString_to_cstr(MZOwnedString *str);

#endif

// Create a string view from c style string.
MZStringView MZStringView_from_cstr(const char *str);

// Create a string view from an address and length.
MZStringView MZStringView_from_addr(const char *addr, size_t len);

#ifndef MIZUKI_STRING_NO_ALLOC

// Create a string view from owned string.
MZStringView MZStringView_from_str(MZOwnedString *str);

#endif

// Invalid string view.
MZStringView MZStringView_invalid();

// Check if string view is empty.
bool MZStringView_is_empty(MZStringView sv);

// Check if string view is invalid.
bool MZStringView_is_invalid(MZStringView sv);

// Remove a character from beginning and return it.
// If nothing to remove, '\0' will be returned.
char MZStringView_chop_char(MZStringView *sv);

// Remove a character from end and return it.
// If nothing to remove, '\0' will be returned.
char MZStringView_rchop_char(MZStringView *sv);

// Drop character from beginning while predicate returns true.
void MZStringView_drop_while(MZStringView *sv, bool (*pred)(char));

// Drop character from end while predicate returns true.
void MZStringView_rdrop_while(MZStringView *sv, bool (*pred)(char));

// Check if the content of sv and rhs are the same.
bool MZStringView_equal(MZStringView sv, MZStringView rhs);

// Find given string view.
MZStringView MZStringView_find(MZStringView sv, MZStringView pat, MZStringView *remain);

#ifdef __cplusplus
}
#endif

#undef MZKprivate__FORMAT_ATTRIB_

#endif // ONION27_MIZUKI_STR_H_

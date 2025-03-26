# Mizuki Strings

Mizuki strings contains a bunch of utilities to help you dealing with strings (and string views) in C.

Most operations operates around `MZOwnedString` and `MZStringView`. `MZOwnedString` represents strings that allocated on heap and provides functions to create and destroy them. On the other hand, `MZStringView` is a view of string that can be from anywhere, they don't own any memory. Please be super careful when dealing with `MZStringView`s and make sure their owner's lifetime must be longer than themselves.

## Build Requirements

- Any compiler that supports at least C23.
- `libc`.

## Build

Just regular CMake build.

```shell
cmake -B build
cmake --build build
```

You can also `FetchContent` and use the `mizukistr` target.

## Example

See examples folder.


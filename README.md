# Codebase for C Programming

Mainly made to use as few libraries as possible and for my own learning and exploration purposes.

## Platform assumptions

This project targets conventional C23 platforms rather than every conforming C implementation. Fundamental integer aliases use deliberately selected built-in C types, with compile-time assertions rejecting platforms where those types have the wrong width. In particular, `i32` is `int` and `u32` is `unsigned int`, and both are required to be 32 bits. This keeps ordinary formatting predictable: use `%d` for `i32` and `%u` for `u32`.

Supporting a platform with different fundamental type widths requires revisiting the aliases and their formatting conventions; it is not handled silently.

## What does this codebase have?

```
codebase/
    src/                        // contains source code
        base/                       // types and helpers with no project dependencies
        memory/                     // allocation strategies and traversal algorithms
        collections/                // data structures and ownership rules
        algorithms/                 // searching, sorting, and traversal algorithms
        fmt/                        // string views, builders, parsing and formatting
        io/                         // portable file and stream operations
        diagnostics/                // logging, named trace channels, and diagnostic output policy
        platform/                   // operating system specific implementations
    tests/                      // contains tests
    programs/                   // runnable demos and complete command-line tools
    build/                      // generated files (.gitignore)
    README.md
    Makefile
```

Every API should answer:
1. who owns the memory?
2. who releases it?
3. how long does a returned pointer remain valid?
4. can this operation invalid existing pointers?

Conventions:
- `init`: initialise an existing object
- `deinit`: release its owned resources
- `clear`: remove contents while retaining reusable capacity
- a view borrows memory and never frees it

Owning structs must not be casually copied by assignment: that can create two apparent owners of one allocation

1. log and named traces [variadic functions, formatting, observable execution]
2. string, array, memory [pointers, arrays, allocation]
3. vec, list, stack, queue [data structures, ownership]
4. hashmap, hashset [hashing, collisions, resizing]
5. arena, pool [memory management]
6. file, path [OS/file system interaction]
7. thread, mutex [concurrency]
8. socket [networking]
9. algorithms [sorting, searching]

Logging is mandatory infrastructure. Applicable data structures and algorithms expose a disabled-by-default named trace channel, such as `binary-search-trace` or `vectorI32-trace`, so their internal decisions can be observed while learning.

Naming conventions
- lowercase aliases for fundamental types
- PascalCase for aggregate types
- snake_case for functions

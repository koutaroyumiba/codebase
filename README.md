# Codebase for C Programming

Mainly made to use as few libraries as possible and for my own learning and exploration purposes.

## What does this codebase have?

```
codebase/
    src/                        // contains source code
        base/
        memory/
        collections/
        algorithms/
        text/
        io/
        diagnostics/
        platform/
    tests/                      // contains tests
    examples/                   // demonstrates individual modules
    programs/                   // programs
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

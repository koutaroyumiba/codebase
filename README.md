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

1. string, array, memory [pointers, arrays, allocation]
2. vec, list, stack, queue [data structures, ownership]
3. hashmap, hashset [hashing, collisions, resizing]
4. arena, pool [memory management]
5. file, path [OS/file system interaction]
6. log [variadic functions, formatting]
7. thread, mutex [concurrency]
8. socket [networking]
9. algorithms [sorting, searching]

Naming conventions
- lowercase aliases for fundamental types
- PascalCase for aggregate types
- snake_case for functions

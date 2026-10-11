# Codebase

A multi-language learning workspace for programming, data structures, algorithms, memory, and systems concepts.

Each language is kept in a self-contained directory so it can use its own idioms, compiler settings, tests, programs, and learning plan. Implementations do not need to remain symmetrical.

## Tracks

- [`c/`](c/) — the active C23 track.
- `cpp/` — planned C++ track; create it when its first concrete learning milestone is chosen.

Cross-language comparison and interoperability can be introduced later through explicit, documented boundaries.

## Commands

The root Makefile currently delegates to the C track:

```sh
make
make test
make run
make clean
```

The same commands can be run directly with `make -C c`.

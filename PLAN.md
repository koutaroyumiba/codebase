# Codebase: Zig-first DSA visualizer with focused C systems labs

This repository is a learning project for data structures, algorithms, computer-science concepts, visualization, low-level memory, and language interoperability.

Zig is the primary implementation language. C is used for focused low-level exercises where manual representation, allocation, ABI design, or platform interaction is itself the lesson. The C work is not required by the normal Zig build.

The previous C-only curriculum is preserved in `PLAN_C_LEGACY.md` for reference.

---

## 1. Project goals

1. Implement data structures and algorithms rather than hiding them behind standard containers.
2. Make internal decisions observable through deterministic structured events.
3. Build a terminal visualizer quickly, followed by a deliberately small graphical frontend.
4. Use Zig for the main application, algorithms, collections, and visualization.
5. Use C for selected systems-programming labs.
6. Optionally integrate stable C labs into Zig through FFI.
7. Keep ownership, failure behavior, invariants, and complexity explicit in both languages.

The goal is not to duplicate the complete codebase in two languages.

---

## 2. Language and dependency policy

### Zig is primary

Use Zig for:

- searching and sorting;
- vectors, queues, heaps, maps, trees, and graphs;
- graph algorithms;
- structured algorithm events;
- terminal and graphical visualization;
- command-line programs;
- application composition and integration tests.

Standard-library allocators, testing, I/O, and platform facilities may support the work. Do not use a standard collection as the implementation of a collection currently being studied.

### C is a focused learning track

Good C labs include:

- checked allocation-size arithmetic;
- fixed-buffer arenas;
- pool allocators;
- raw dynamic storage as a comparison exercise;
- borrowed string views and owned byte strings;
- byte readers/writers;
- selected platform and ABI experiments.

A C lab must have a concrete learning objective. Do not create a C version merely to keep the languages symmetrical.

### Dependencies

- The default Zig build must not require the C labs.
- The C labs must build and test independently.
- FFI integration is opt-in.
- A graphical dependency requires explicit approval before it is added.
- Prefer no external dependencies until the terminal visualizer works.

---

## 3. Optional FFI policy

FFI means Zig may call a stable C module through a C-compatible API. It is an optional extension, never a prerequisite for the main algorithms or visualizer.

Conceptual build modes:

```text
zig build test                 Zig unit and integration tests
make -C c test                 standalone C lab tests
zig build test -Dffi=true      optional Zig/C integration tests
```

The exact Zig build option will be implemented only when the first FFI milestone begins.

### Boundary rules

1. Memory is normally released by the module that allocated it.
2. Borrowed buffers cross the boundary as a pointer and length with a documented lifetime.
3. C APIs expose only C-compatible functions, structs, integer types, pointers, lengths, and status values.
4. Zig slices, error unions, optionals, and generic types do not cross the C ABI directly.
5. Zig wrappers translate C status values into idiomatic Zig errors and pointer/length pairs into slices where safe.
6. C must not retain Zig-owned memory unless the API explicitly documents the lifetime.
7. C failures must not terminate the Zig application.
8. Each C module passes native C tests before receiving an FFI wrapper.
9. Optional FFI behavior must be tested against the standalone C behavior.
10. The normal Zig application remains usable when FFI is disabled.

When a C backend replaces behavior also available in Zig, keep a Zig backend as the default. When a C lab has no production use, expose it only through a focused FFI demo or integration test.

---

## 4. Intended repository structure

Create directories only when their first real module is started.

```text
codebase/
├── build.zig
├── README.md
├── PLAN.md
├── PLAN_C_LEGACY.md
├── src/
│   ├── root.zig
│   ├── algorithms/
│   ├── collections/
│   ├── visualization/
│   ├── render/
│   ├── platform/
│   └── programs/
├── tests/
├── fixtures/
└── c/
    ├── include/codebase/
    ├── src/
    ├── tests/
    └── Makefile
```

The existing C files remain in place until the Zig foundation is working and a separate migration step is approved. Do not combine repository movement, build replacement, and algorithm ports into one change.

---

## 5. Architecture

### Main dependency direction

```text
Programs / visualizer
        ↓
Visualization adapters and replay
        ↓
Algorithms and collections
        ↓
Small Zig foundations and standard library
```

Rendering and platform code may depend on visualization data. Algorithms and collections must not depend on a renderer, window system, terminal control system, or graphical library.

### Observation model

Algorithms may accept an optional borrowed observer that receives structured events. Examples include:

- comparisons and swaps;
- active indexes and ranges;
- merge splits and completed ranges;
- queue frontier changes;
- heap sift operations;
- hash probes and collisions;
- graph visits and edge relaxation.

Requirements:

- ordinary APIs remain available without an observer;
- observed and unobserved execution produce identical results;
- events are deterministic for deterministic input;
- callbacks do not retain borrowed data unless the contract explicitly transfers ownership;
- observation does not allocate unless an explicit recorder chooses to copy an event;
- begin with module-specific events instead of one universal event hierarchy.

Text logging remains useful for humans but must not be parsed as the visualizer's data source.

---

## 6. Learning workflow

For each milestone:

1. Read the contract and predict edge cases.
2. Add focused tests and observe the initial failure.
3. Implement the smallest understandable solution.
4. Run focused tests until they pass.
5. Inspect structured events or trace a small example by hand.
6. Explain ownership, invariants, failure behavior, and complexity.
7. Request review before activating the next milestone.

A reusable module is complete when it has:

- a focused public API;
- documented preconditions and failure behavior;
- explicit ownership and borrowing rules;
- tests for boundaries and representative failures;
- complexity notes where relevant;
- useful observation events where internal decisions matter;
- passing focused tests and applicable safety checks.

---

# Roadmap

This roadmap retains every numbered milestone from the original plan. `PLAN_C_LEGACY.md` remains the detailed specification for each original goal, concept list, required test, and acceptance criterion. Unless this plan explicitly changes a language-specific mechanism, those requirements still apply.

For Zig milestones, translate C-specific API mechanics idiomatically while preserving the learning outcome. For example, pointer/length inputs become slices and return booleans may become error unions. For C milestones, retain the original C ownership and failure requirements.

Visualization milestones are additions. They do not replace or remove original work.

## Phase 0 — Preserve the C checkpoint

The existing C implementation contains base types, diagnostics, vector operations, searching, insertion sort, and merge sort. Keep it working while establishing Zig. It is a historical and comparative checkpoint, not the architecture of the new primary application.

Do not move or rewrite the existing C tree until the Zig build and first port are working.

## Zig bootstrap — before numbered milestones

### Bootstrap A — Build and test skeleton

- Add `build.zig` and `src/root.zig`.
- Add one minimal unit test.
- Establish debug and test commands.
- Record the Zig version because build APIs can change between releases.
- Add no third-party packages.

### Bootstrap B — Existing foundation in Zig

Port the capabilities that existed before the original numbered roadmap:

- deterministic logging and named trace channels;
- linear search and binary search;
- an initial concrete `VectorI32` with initialization, cleanup, reserve, push, pop, and clear;
- ownership, borrowing, failure, and pointer-invalidation documentation.

Structured visualization events may share concepts with textual tracing, but the visualizer must not parse log messages.

---

# Part I — Arrays and fundamental algorithms

## Milestone 1 — Insertion sort — Zig

Implement stable in-place insertion sort over an `i32` slice. Preserve the original boundary, ordering, stability, complexity, and trace requirements.

## Milestone 2 — Merge sort — Zig

Implement stable merge sort with an explicit allocator and one temporary allocation. Allocation or size failure must not corrupt the caller's input.

## Visualization fast lane — additional milestones

Complete these after Milestone 2 and before Milestone 3.

### V1 — Structured sorting and searching events — Zig

- Add module-specific events for comparisons, moves, placements, active ranges, splits, merges, and binary-search narrowing.
- Keep ordinary unobserved APIs.
- Verify that observed and unobserved runs return identical results.
- Keep event order deterministic for deterministic input.

### V2 — Terminal DSA visualizer — Zig

- Display array state, active indexes or ranges, and the current operation.
- Support deterministic single-step replay first.
- Add pause, restart, and speed controls only after stepping works.
- Keep terminal presentation outside algorithm modules.

### V3 — Minimal graphical visualizer — Zig

After explicitly approving a graphical dependency, add only frame lifecycle, input, timing, rectangles, lines, text, colors, and basic playback controls. Initially visualize insertion sort, merge sort, and binary search.

Do not add a scene graph, ECS, asset pipeline, shader framework, or multiple rendering backends without a demonstrated need.

## Milestone 3 — Vector insertion and removal — Zig

Extend the concrete Zig vector with ordered indexed insertion and removal, preserving overlap safety, failure-state validity, and pointer-invalidation rules.

## Milestone 4 — Number-sorting CLI — Zig

Build the original complete sorting program with parsing, range errors, process status, cleanup, and reusable modules. It may optionally launch or feed the visualizer.

---

# Part II — Core collections — Zig

## Milestone 5 — Stack

Expose a clear LIFO interface using vector storage without duplicating the dynamic-array implementation.

## Milestone 6 — Ring-buffer queue

Implement a growable FIFO queue with wraparound, growth while wrapped, and explicit head/length/capacity invariants. Add events that can visualize logical and physical layout.

## Milestone 7 — Singly linked list

Implement node ownership, front operations, traversal, removal, allocation failure, and complete cleanup. Compare noncontiguous storage with vectors.

## Milestone 8 — Binary heap / priority queue

Implement a vector-backed `i32` min-heap with sift-up and sift-down. Visualize both array and tree representations.

## Milestone 9 — Hash map

Implement a concrete `i32`-to-`i32` map with collision handling, deletion, resizing, and explicit empty/deleted slot representation. Add probe, collision, and resize events.

## Milestone 10 — Bitset

Implement compact boolean membership with correct masks, shifts, word-boundary behavior, padding policy, and safe storage calculations.

---

# Part III — Memory management — C

These are standalone C labs with native C tests. They do not become default dependencies of the Zig application.

## Milestone 11 — Checked size arithmetic

Implement reusable overflow-safe addition and multiplication for allocation sizes. Preserve unchanged output on failure and pointer-width independence.

## Milestone 12 — Fixed-buffer arena

Implement aligned bump allocation over caller-owned storage, exhaustion, reset, and grouped-lifetime rules. Failed allocation must leave the cursor unchanged.

## Milestone 13 — Allocation interface and failure injection

Implement a small C allocation interface with explicit allocate/resize/free capabilities and a deterministic failure-injection allocator. Adapt at least one C structure so failure-state validity and leak freedom can be tested.

## Milestone 14 — Pool allocator

Implement aligned fixed-size slots, a free list, reuse, reset behavior, and protection or documented assertions for invalid and duplicate release.

---

# Part IV — Text, parsing, and binary data

## Milestone 15 — String view — C

Implement a borrowed pointer/length byte view with construction, equality, prefix/suffix checks, slicing, find, and ASCII trimming. It never owns or frees backing memory.

## Milestone 16 — String builder — C

Implement an owning growable null-terminated byte string with explicit length/capacity rules, overlap policy, allocation failure safety, and pointer invalidation documentation.

## Milestone 17 — Integer parsing and formatting — Zig

Implement decimal `i32` parsing and formatting without delegating the core conversion to a standard convenience parser. Preserve malformed/range distinctions, boundary handling, partial-input policy, and round-trip tests.

The Zig sorting CLI uses this Zig implementation by default; it does not require the C string labs.

## Milestone 18 — Byte reader and writer — C

Implement bounded cursor-based fixed-width integer encoding with explicit endianness, no unaligned integer casts, and unchanged cursors on failure.

A Zig program that needs byte decoding must have a native Zig path or local implementation so FFI remains optional.

---

# Part V — Files and operating-system boundaries — Zig

## Milestone 19 — Whole-file reading and writing

Implement owned whole-file reads and complete writes with empty-file policy, partial-operation handling, size limits, useful errors, and cleanup on every path.

## Milestone 20 — Directory traversal and paths

Create a small platform-isolated directory API with explicit entry-name lifetimes, nondeterministic ordering policy, and reliable iterator cleanup.

## Milestone 21 — Timing and benchmarks

Add monotonic timing and responsibly compare insertion sort with merge sort using deterministic inputs, correctness checks, recorded build mode, and repeated measurements.

## Milestone 22 — Memory mapping — optional

Expose a read-only mapped file as a borrowed byte view with prominent mapping-lifetime and cleanup rules. Keep ordinary owned file reading available.

---

# Part VI — Trees and graph algorithms — Zig

## Milestone 23 — Binary search tree

Implement insertion, lookup, traversal, duplicate policy, and deletion for leaf, one-child, and two-child nodes. Visualize structural changes and sorted-input degeneration.

## Milestone 24 — Graph representation

Implement a directed adjacency-list graph with explicit vertex/edge ownership, invalid-index behavior, duplicate/self-loop policy, and atomic failure behavior.

## Milestone 25 — Breadth-first and depth-first search

Implement queue-based BFS and stack or recursive DFS with visited sets, predecessor data, disconnected-graph behavior, and `O(V + E)` complexity. Emit frontier and visitation events.

## Milestone 26 — Topological sorting

Produce a valid order for DAGs and detect cycles. Tests validate edge constraints rather than requiring one exact valid order.

## Milestone 27 — Dijkstra's shortest paths

Add nonnegative weighted edges, relaxation, a priority queue, stale-entry handling, unreachable representation, overflow safety, and predecessor-based path reconstruction.

---

# Part VII — Larger programs — Zig

## Milestone 28 — Word-frequency counter

Read and tokenize a file, count words, define punctuation/case policy, manage key lifetimes safely, and produce deterministic output. Adapt the Zig map for string-view keys when this real use requires it.

## Milestone 29 — Expression evaluator

Separate tokenization from evaluation and implement precedence, parentheses, useful syntax positions, division-by-zero handling, and an explicit arithmetic-overflow policy.

## Milestone 30 — Pathfinding demo

Load or construct a graph, request endpoints, run shortest-path search, reconstruct the path, and optionally replay its events in the visualizer.

## Milestone 31 — Binary-file inspector

Display offsets, bytes, and explicit-endian decoded values without reading beyond the input. Use a native Zig decoding path by default; the C byte reader may be compared through optional FFI.

---

# Part VIII — Systems-programming branches — Zig primary

These remain optional branches, as in the original plan. Selected C comparisons may be added when they teach a specific platform or ABI lesson.

## Milestone 32 — Threads and mutexes — optional

Build a bounded producer/consumer queue and learn data races, mutexes, condition variables, spurious wakeups, shutdown, deterministic coordination, and ownership transfer.

## Milestone 33 — Sockets — optional

Build a TCP echo client/server with address handling, byte order, framing, partial send/receive loops, disconnect behavior, and complete cleanup.

## Milestone 34 — Processes — optional

Spawn a child process, capture output, inspect exit status, handle missing executables, and avoid pipe deadlocks and inherited-resource leaks.

## Milestone 35 — Second-language implementation and FFI — optional

The repository already uses Zig and C, so this milestone becomes a deliberate comparison and interoperability checkpoint rather than a syntax port.

Possible independent sub-milestones:

- wrap the C arena in Zig using a Zig-owned borrowed buffer;
- wrap the C byte string while keeping allocation and release on the C side;
- compare C and Zig byte readers against shared fixtures;
- compare selected stable data structures in both languages;
- document ABI layout, error translation, ownership, and performance tradeoffs.

Each C module must pass standalone C tests before receiving a Zig wrapper. The default Zig build, algorithms, programs, and visualizer must continue to work with FFI disabled.

---

# Complete order at a glance

Foundation: Zig build/test skeleton, logging/tracing, search, and initial vector.

1. Insertion sort — Zig.
2. Merge sort — Zig.
V1–V3. Structured events, terminal visualizer, graphical visualizer — Zig.
3. Vector insertion/removal — Zig.
4. Number-sorting CLI — Zig.
5. Stack — Zig.
6. Ring-buffer queue — Zig.
7. Singly linked list — Zig.
8. Binary heap — Zig.
9. Hash map — Zig.
10. Bitset — Zig.
11. Checked size arithmetic — C.
12. Fixed-buffer arena — C.
13. Allocation interface and failure injection — C.
14. Pool allocator — C.
15. String view — C.
16. String builder — C.
17. Integer parsing and formatting — Zig.
18. Byte reader/writer — C.
19. Whole-file I/O — Zig.
20. Directory traversal — Zig.
21. Timing and sorting benchmarks — Zig.
22. Memory mapping — optional, Zig.
23. Binary search tree — Zig.
24. Graph representation — Zig.
25. BFS and DFS — Zig.
26. Topological sort — Zig.
27. Dijkstra's algorithm — Zig.
28. Word-frequency counter — Zig.
29. Expression evaluator — Zig.
30. Pathfinding demo — Zig.
31. Binary-file inspector — Zig.
32. Threads and mutexes — optional, Zig primary.
33. Sockets — optional, Zig primary.
34. Processes — optional, Zig primary.
35. Cross-language comparison and FFI — optional.

The order may change when a real program exposes a better dependency order, but a milestone must not disappear when reordered.

---

# Current checkpoint

The active work is **Bootstrap A: Zig build and test skeleton**.

Before implementation:

1. Confirm and install a Zig toolchain; Zig is not currently available on this machine.
2. Choose and record the Zig version.
3. Add only the build skeleton and one test.
4. Keep the existing C build passing and do not move its source tree yet.
5. Do not add a graphical dependency yet.

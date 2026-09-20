# Codebase: bottom-up C, data structures, and algorithms curriculum

This repository is a long-running learning project. The goal is not to finish a general-purpose library quickly. The goal is to build increasingly capable programs while learning how their foundations work.

Build one milestone at a time. Do not implement later milestones early, and do not generalize a concrete implementation until a second use case exposes useful repetition.

---

## 1. Learning workflow

For every milestone:

1. Read the public behavior and predict important edge cases.
2. Add or enable only that milestone's tests.
3. Run the tests and observe the expected failure (**red**).
4. Implement the smallest understandable solution yourself.
5. Run tests until they pass (**green**).
6. Trace at least one example by hand.
7. Explain ownership, invariants, failure behavior, and complexity.
8. Request review before moving forward.

The agent may supply tests and review code, but you should write the implementation. Tests for distant milestones should not be added to the build early: their missing symbols would prevent current tests from running, and their APIs may change based on what earlier programs teach us.

### Definition of done for a module

A reusable module is done when it has:

- a focused public header;
- documented preconditions and postconditions;
- explicit ownership and pointer-lifetime rules;
- defined failure behavior;
- tests for normal behavior and meaningful edge cases;
- time and space complexity notes where relevant;
- a small example or use in a real program;
- passing `make test` and sanitizer checks when available.

---

## 2. Current repository

```text
codebase/
├── Makefile
├── README.md
├── tmp.md
├── src/
│   ├── base/
│   │   ├── base.h
│   │   ├── constants.h
│   │   ├── types.h
│   │   └── utils.h
│   ├── collections/
│   │   ├── vector.h
│   │   └── vector.c
│   ├── algorithms/
│   │   ├── search.h
│   │   └── search.c
│   └── main.c
├── tests/
│   └── test_main.c
└── build/
```

Already implemented:

- C23 build with warnings enabled;
- fixed-width integer aliases;
- a minimal test runner;
- an owning `VectorI32` with `init`, `deinit`, `reserve`, `push`, `pop`, and `clear`;
- linear search;
- binary search.

Do not rewrite working modules just to make them generic. Extend them when a milestone requires it.

---

## 3. Architecture and conventions

### Dependency direction

```text
Programs
   ↓
Algorithms / text / I/O
   ↓
Collections / memory / platform
   ↓
Base
   ↓
C standard library and native OS APIs
```

A module only includes what it actually needs. A sorting algorithm may depend directly on `base`; it does not need to pass through `collections`.

### Directory responsibilities

- `src/base/`: types and tiny helpers with almost no project dependencies.
- `src/collections/`: data structures and their ownership rules.
- `src/algorithms/`: searching, sorting, and traversal algorithms.
- `src/memory/`: allocation strategies and checked size calculations.
- `src/text/`: string views, builders, parsing, and formatting.
- `src/io/`: portable file and stream operations.
- `src/platform/`: operating-system-specific implementations.
- `programs/`: complete command-line tools.
- `examples/`: minimal demonstrations of individual modules.
- `tests/`: automated behavior tests.

Create a directory only when its first module is started.

### API rules

Every owning API must answer:

1. Who owns allocated memory?
2. Who releases it?
3. How long is a returned pointer valid?
4. Which operations invalidate existing pointers?

Naming conventions already established by the repository:

- `init`: initialize an existing object;
- `deinit`: release owned resources;
- `clear`: remove values while retaining reusable storage;
- a `view`: borrows memory and never frees it;
- `bool`: success/failure when there is only one useful failure category;
- an enum: failure result when callers need to distinguish causes.

Use `assert` for programmer errors such as violating a documented precondition. Return an error for normal runtime failures such as allocation failure, malformed input, or a missing file.

### Build progression

Keep the current build simple. Add these targets only when needed:

```text
make
make run
make test
make debug
make release
make sanitize
make clean
```

Do not introduce external dependencies or a larger build system unless the project reaches a limitation that can be demonstrated.

---

# Part I — Arrays and fundamental algorithms

These milestones build fluency with contiguous memory, indexes, invariants, and complexity before introducing more pointer-heavy structures.

## Milestone 1 — Insertion sort (active)

### Goal

Sort an `i32` array in ascending order in place.

### Files to create or change

- Create `src/algorithms/sort.h`.
- Create `src/algorithms/sort.c`.
- Tests already exist in `tests/test_main.c`.

### Concepts to understand

- sorted-prefix loop invariant;
- in-place mutation;
- stable sorting;
- unsigned indexes;
- best-case `O(n)`, average/worst-case `O(n²)`, extra space `O(1)`.

### Build steps

1. Declare `void insertion_sort_i32(i32 *data, usize len)` and document its contract.
2. Implement the sorted-prefix algorithm without allocation.
3. Trace mixed, sorted, reverse, and duplicate inputs by hand.

### Required tests

- null pointer with zero length;
- one element;
- mixed positive and negative values;
- duplicates;
- already sorted input;
- reverse-sorted input;
- all-equal input;
- `I32_MIN` and `I32_MAX`.

### Acceptance criteria

- `make test` passes;
- output is nondecreasing and preserves all input values;
- no allocation occurs;
- you can explain why shifting on `>` preserves stability.

### Optional hints

Save the current value before shifting larger values. Never decrement an unsigned index below zero.

## Milestone 2 — Merge sort

### Goal

Implement a stable `i32` merge sort and compare it with insertion sort.

### Files to create or change

- Change `src/algorithms/sort.h`.
- Change `src/algorithms/sort.c`.
- Add merge-sort cases to `tests/test_main.c` when this milestone begins.

### Concepts to understand

- divide and conquer;
- recursion and base cases;
- merging two sorted ranges;
- allocation failure;
- time `O(n log n)` and extra space `O(n)`.

### Build steps

1. First write a helper that merges two adjacent sorted ranges into temporary storage.
2. Recursively sort the left and right halves.
3. Allocate temporary storage once at the public entry point, not once per recursive call.
4. Return `bool` so allocation failure can be reported without corrupting input.

### Required tests

Reuse all insertion-sort inputs, then add:

- lengths immediately below and above powers of two;
- a larger deterministic input;
- allocation-size overflow rejection if the API permits synthetic huge lengths;
- allocation failure leaving the caller's array valid.

### Acceptance criteria

- result matches insertion sort or `qsort` on deterministic inputs;
- duplicate values are handled correctly;
- temporary memory is always freed;
- failure behavior is documented;
- you can draw the recursive split/merge tree for eight values.

### Optional hints

Use half-open ranges `[begin, end)`. They make lengths and empty ranges easier to calculate.

## Milestone 3 — Vector insertion and removal

### Goal

Add indexed insertion and removal to `VectorI32`.

### Files to create or change

- Change `src/collections/vector.h`.
- Change `src/collections/vector.c`.
- Add tests to `tests/test_main.c`.

### Concepts to understand

- moving overlapping memory;
- index preconditions;
- growth invalidating pointers;
- ordered removal versus swap removal;
- operation cost `O(n)`.

### Build steps

1. Add ordered insertion at indexes from zero through `len`.
2. Add ordered removal for indexes below `len`.
3. Return the removed value using the existing option style.
4. Consider swap removal only as a later, separately named operation.

### Required tests

- insert into an empty vector;
- insert at front, middle, and end;
- insertion that triggers growth;
- remove front, middle, and final element;
- preserved order;
- repeated insertion/removal;
- invalid indexes follow the documented assertion contract.

### Acceptance criteria

- `len <= cap` always holds;
- insertion failure leaves the vector unchanged;
- moved ranges do not corrupt overlapping elements;
- ownership behavior remains unchanged.

### Optional hints

Read the contract of `memmove`; unlike `memcpy`, it supports overlapping ranges.

## Milestone 4 — Number-sorting CLI

### Goal

Build the first complete program: read integers from command-line arguments, sort them, and print them.

### Files to create or change

- Create `programs/sort_numbers.c`.
- Change `Makefile` to add a focused program target.
- Add a CLI test script or C integration test under `tests/`.

### Concepts to understand

- `argc` and `argv`;
- text-to-integer conversion;
- malformed input and range errors;
- composing vector and sorting modules;
- process exit status.

### Required tests

- no numbers;
- one number;
- mixed numbers and duplicates;
- malformed text;
- values outside the `i32` range;
- output formatting;
- nonzero status on invalid input.

### Acceptance criteria

- the program does not leak on any exit path;
- valid values print in ascending order;
- invalid input produces a useful error on standard error;
- modules are reused rather than duplicated in the program.

### Optional hints

Start with standard conversion facilities. Reimplement integer parsing later, after behavior is understood.

---

# Part II — Core collections

Build representations in increasing order of pointer and invariant complexity.

## Milestone 5 — Stack

### Goal

Expose clear last-in, first-out operations using vector storage.

### Files to create or change

- Initially add documentation/example code using `VectorI32` directly.
- Add `src/collections/stack.h/.c` only if a separate API improves a real caller.
- Add tests when the API exists.

### Concepts to understand

- LIFO behavior;
- abstraction versus representation;
- reusing `push` and `pop`;
- `O(1)` amortized push and `O(1)` pop.

### Required tests

- empty pop;
- one item;
- several pushes followed by reverse-order pops;
- clear and reuse;
- growth through the underlying vector.

### Acceptance criteria

- no duplicate dynamic-array implementation;
- ownership is explicit;
- an expression-evaluator milestone can use the interface naturally.

### Optional hints

A data structure can be an API convention over an existing representation.

## Milestone 6 — Ring-buffer queue

### Goal

Implement a growable first-in, first-out queue of `i32` values.

### Files to create or change

- Create `src/collections/queue.h`.
- Create `src/collections/queue.c`.
- Add queue tests.

### Concepts to understand

- FIFO behavior;
- head, length, and capacity invariants;
- wraparound indexing;
- growing wrapped data into a contiguous new buffer;
- amortized `O(1)` enqueue and `O(1)` dequeue.

### Required tests

- empty dequeue;
- enqueue/dequeue one value;
- FIFO order;
- wraparound without growth;
- growth while wrapped;
- fill, drain, and reuse;
- overflow and allocation failure preserving validity.

### Acceptance criteria

- logical order is independent of physical layout;
- indexes never exceed capacity;
- growth preserves order;
- all memory is released by `deinit`.

### Optional hints

Store `head` and `len`; derive the insertion index from them instead of maintaining several redundant fields.

## Milestone 7 — Singly linked list

### Goal

Implement a singly linked list of `i32` values with front operations and traversal.

### Files to create or change

- Create `src/collections/list.h`.
- Create `src/collections/list.c`.
- Add list tests.

### Concepts to understand

- nodes and links;
- heap ownership per node;
- pointer-to-pointer manipulation;
- contiguous versus noncontiguous storage;
- `O(1)` front insertion and `O(n)` indexed access.

### Required tests

- empty list;
- push/remove front;
- traversal order;
- remove middle and absent value;
- clear and reuse;
- allocation failure;
- cleanup of every node.

### Acceptance criteria

- no node is leaked or freed twice;
- head/tail invariants are documented if a tail is stored;
- failed insertion leaves the list unchanged;
- you can compare list and vector cache behavior conceptually.

### Optional hints

Begin with only a head pointer. Add a tail only when an operation needs it.

## Milestone 8 — Binary heap / priority queue

### Goal

Implement a min-heap of `i32` values backed by a vector-like array.

### Files to create or change

- Create `src/collections/heap.h`.
- Create `src/collections/heap.c`.
- Add heap tests.

### Concepts to understand

- complete binary trees in arrays;
- parent and child index calculations;
- heap ordering invariant;
- sift-up and sift-down;
- `O(log n)` insertion/removal and `O(1)` minimum lookup.

### Required tests

- empty peek/pop;
- one value;
- ascending, descending, and duplicate insertions;
- repeated pop produces sorted order;
- growth;
- index calculations near boundaries.

### Acceptance criteria

- every parent is no larger than its children;
- pop repairs the invariant;
- duplicates are retained;
- memory and failure rules are documented.

### Optional hints

Write the parent/child formulas on paper and check them for indexes zero through six before coding.

## Milestone 9 — Hash map

### Goal

Implement a concrete `i32`-to-`i32` hash map before attempting generic or string-keyed maps.

### Files to create or change

- Create `src/collections/i32_map.h/.c`.
- Add map tests.

### Concepts to understand

- hash functions;
- load factor;
- collisions;
- open addressing or separate chaining;
- deletion markers;
- resizing.

### Required tests

- empty lookup;
- insert and retrieve;
- overwrite/update;
- negative and extreme keys;
- deliberate collisions;
- resize with all entries preserved;
- deletion and reinsertion;
- missing key;
- allocation failure.

### Acceptance criteria

- collision handling is correct;
- resizing preserves every entry;
- average expected lookup is documented as `O(1)` and worst case as `O(n)`;
- the empty-slot and deleted-slot representations cannot be confused with valid keys.

### Optional hints

Choose one collision strategy and learn it thoroughly. Add string-view keys later, after string lifetimes are understood; do not make the map generic yet.

## Milestone 10 — Bitset

### Goal

Store boolean membership compactly in machine words.

### Files to create or change

- Create `src/collections/bitset.h/.c`.
- Add bitset tests.

### Concepts to understand

- bits, bytes, and words;
- masks and shifts;
- set, clear, toggle, and test;
- rounding bit counts to storage words.

### Required tests

- zero bits;
- first and last valid bit;
- indexes around word boundaries;
- set/clear/toggle;
- all bits operations if implemented;
- allocation-size overflow.

### Acceptance criteria

- unrelated bits do not change;
- padding bits are handled consistently;
- out-of-range behavior is documented;
- storage size calculations cannot overflow.

### Optional hints

Derive a word index and an offset within that word separately.

---

# Part III — Memory management

Do this after several allocating structures have exposed repeated problems. Memory utilities support modules; they are not a prerequisite for learning every collection.

## Milestone 11 — Checked size arithmetic

### Goal

Centralize safe addition and multiplication for allocation sizes.

### Files to create or change

- Create `src/memory/checked.h/.c`, or keep tiny typed helpers in `base` if no runtime dependency is needed.
- Refactor one existing allocator call at a time.
- Add focused tests.

### Concepts to understand

- unsigned overflow;
- prechecking before multiplication/addition;
- why wrapped allocation sizes can cause buffer overflow.

### Required tests

- zero operands;
- ordinary values;
- exact maximum representable result;
- one-past-maximum failure;
- output parameter unchanged on failure.

### Acceptance criteria

- no overflowing operation is performed before checking it;
- vector, queue, and later allocators can reuse the helpers;
- behavior is independent of a specific pointer width.

### Optional hints

For multiplication, handle zero and compare one operand with `SIZE_MAX / other` before multiplying.

## Milestone 12 — Fixed-buffer arena

### Goal

Allocate aligned regions from a caller-supplied buffer and release them together with `reset`.

### Files to create or change

- Create `src/memory/arena.h`.
- Create `src/memory/arena.c`.
- Add arena tests.

### Concepts to understand

- alignment;
- cursor/bump allocation;
- borrowed backing storage;
- grouped lifetimes;
- exhaustion and reset;
- no individual free.

### Required tests

- zero-sized or empty backing buffer;
- sequential allocations;
- requested alignments;
- exact exhaustion and one byte too many;
- arithmetic overflow;
- failed allocation leaves cursor unchanged;
- reset reuses storage;
- earlier pointers become logically invalid after reset/reuse.

### Acceptance criteria

- returned addresses satisfy supported alignment;
- arena never writes outside the backing buffer;
- reset does not free caller-owned memory;
- behavior for zero-size allocation is defined.

### Optional hints

First implement alignment for powers of two and assert that precondition.

## Milestone 13 — Allocation interface and failure injection

### Goal

Allow selected collections to allocate through a small interface so tests can deterministically simulate failures.

### Files to create or change

- Create `src/memory/allocator.h/.c`.
- Add a test allocator under `tests/`.
- Adapt one collection before adapting others.

### Concepts to understand

- function pointers and context pointers;
- dependency injection;
- realloc semantics;
- allocator capability mismatch;
- deterministic failure testing.

### Required tests

- normal heap allocation;
- fail on the first and later allocation calls;
- collection remains valid after failure;
- no leaked successful allocations;
- unsupported arena-style operations are not silently treated as heap operations.

### Acceptance criteria

- the interface documents allocate/resize/free capabilities;
- heap and arena differences remain explicit;
- collection failure paths become testable without requesting enormous real allocations.

### Optional hints

Do not force a simple arena to pretend it supports individual frees. Design from actual caller needs.

## Milestone 14 — Pool allocator

### Goal

Allocate and recycle many fixed-size objects efficiently.

### Files to create or change

- Create `src/memory/pool.h/.c`.
- Add pool tests.

### Concepts to understand

- free lists;
- fixed-size slots;
- alignment;
- object lifetime and double-free hazards.

### Required tests

- allocate until full;
- free and reuse a slot;
- alignment;
- reject or assert foreign pointers;
- reset behavior;
- no duplicate live allocation of one slot.

### Acceptance criteria

- every slot is either live or in the free list, never both;
- operations are `O(1)`;
- supported object size/alignment are documented.

### Optional hints

The bytes of a free slot can store the link to the next free slot if the slot is large and aligned enough.

---

# Part IV — Text, parsing, and binary data

Keep byte-oriented text separate from full Unicode processing. These milestones initially operate on bytes and ASCII-aware rules.

## Milestone 15 — String view

### Goal

Represent borrowed text as a pointer and length without requiring null termination.

### Files to create or change

- Create `src/text/string_view.h/.c`.
- Add string-view tests.

### Concepts to understand

- borrowed memory;
- pointer plus length;
- slices and half-open ranges;
- null-terminated strings versus arbitrary byte spans;
- lifetime dependency on backing storage.

### Operations

- construct from pointer/length and C string;
- equality;
- prefix and suffix checks;
- slicing;
- find;
- ASCII whitespace trimming.

### Required tests

- empty view;
- equal and unequal views;
- embedded null bytes for length-based operations;
- prefix/suffix at exact length;
- valid empty slices at boundaries;
- missing search value;
- trimming empty/all-whitespace/mixed input;
- no operation reads beyond `len`.

### Acceptance criteria

- views never free memory;
- returned subviews point into the original storage;
- no function assumes null termination unless named/documented to do so;
- lifetime rules are documented.

### Optional hints

Implement equality before searching or splitting. Keep constructors separate from operations.

## Milestone 16 — String builder

### Goal

Build an owning, growable, null-terminated byte string.

### Files to create or change

- Create `src/text/string_builder.h/.c`.
- Add builder tests.

### Concepts to understand

- logical length versus capacity;
- trailing null terminator;
- append growth;
- exposing borrowed pointers;
- self-append and overlapping input.

### Required tests

- empty builder exposes an empty C string;
- append character and view;
- repeated growth;
- terminator exists after every success;
- clear and reuse;
- allocation failure preserves validity;
- append containing embedded null if supported;
- overlapping/self-append behavior is defined.

### Acceptance criteria

- `len` excludes the terminator;
- capacity rules clearly state whether terminator space is included;
- `data[len] == '\0'` after successful operations;
- growth may invalidate exposed pointers and is documented.

### Optional hints

Reserve space for content plus one terminator, checking that addition for overflow.

## Milestone 17 — Integer parsing and formatting

### Goal

Parse decimal `i32` values from string views and format them without relying on `atoi`.

### Files to create or change

- Create `src/text/parse.h/.c`.
- Create `src/text/format.h/.c` if formatting is kept separate.
- Add tests.

### Concepts to understand

- validation versus conversion;
- overflow before multiply/add;
- signs and boundaries;
- returning an error plus consumed length;
- formatting negative minimum safely.

### Required tests

- zero, positive, and negative values;
- `I32_MIN` and `I32_MAX`;
- one beyond each limit;
- empty input and sign only;
- invalid character;
- leading/trailing whitespace policy;
- partial parse policy;
- formatting round trip.

### Acceptance criteria

- malformed input is distinguishable from range overflow if callers need it;
- no signed overflow occurs during parsing or formatting;
- the sorting CLI can replace standard conversion code with this module.

### Optional hints

Accumulating in a negative range can simplify handling `I32_MIN`, whose positive magnitude is not representable as `i32`.

## Milestone 18 — Byte reader and writer

### Goal

Read and write fixed-width integers from byte buffers with explicit endianness.

### Files to create or change

- Create `src/io/byte_reader.h/.c`.
- Create `src/io/byte_writer.h/.c`.
- Add tests.

### Concepts to understand

- endianness;
- byte shifts and masks;
- cursor-based parsing;
- unaligned access;
- bounds checking.

### Required tests

- little- and big-endian known byte patterns;
- every supported integer width;
- exact buffer boundary;
- truncated input/output;
- failure leaves cursor unchanged;
- round-trip encode/decode;
- values with high bits set.

### Acceptance criteria

- implementation does not depend on host endianness;
- no out-of-bounds access occurs;
- failure behavior is consistent across operations.

### Optional hints

Construct integers from individual bytes rather than casting an arbitrary byte pointer to an integer pointer.

---

# Part V — Files and operating-system boundaries

Start portable, then isolate platform-specific behavior.

## Milestone 19 — Whole-file reading and writing

### Goal

Read an entire file into an owning buffer and write a complete buffer to a file.

### Files to create or change

- Create `src/io/file.h/.c`.
- Add tests using temporary files under the test environment.

### Concepts to understand

- resource acquisition and cleanup;
- file sizes versus memory sizes;
- empty files;
- partial reads/writes;
- binary versus text mode;
- ownership of returned buffers.

### Required tests

- empty file;
- text and binary data including null bytes;
- nonexistent file;
- write then read round trip;
- file open/read/write failure;
- configured size limit;
- all handles and buffers cleaned up on failure.

### Acceptance criteria

- returned buffer ownership is explicit;
- empty files succeed with a defined representation;
- partial operations are handled rather than assumed impossible;
- error categories are useful to callers.

### Optional hints

Use one cleanup path if it makes every acquired resource easy to audit.

## Milestone 20 — Directory traversal and paths

### Goal

List directory entries through a small platform-isolated API.

### Files to create or change

- Create a portable declaration in `src/io/` or `src/platform/`.
- Create the current OS implementation under `src/platform/`.
- Add filesystem integration tests.

### Concepts to understand

- platform APIs;
- path separators and special entries;
- borrowed versus owned entry names;
- iteration state and cleanup;
- nondeterministic directory order.

### Required tests

- empty temporary directory;
- files and subdirectories;
- nonexistent path;
- names with spaces;
- tests do not assume enumeration order;
- iterator cleanup on early exit.

### Acceptance criteria

- native API details do not leak through unrelated modules;
- all OS handles are closed;
- another platform could supply the same public contract later.

### Optional hints

Sort entries in a caller if deterministic display is needed; directory APIs need not promise order.

## Milestone 21 — Timing and benchmarks

### Goal

Add monotonic high-resolution timing and use it to compare sorting algorithms responsibly.

### Files to create or change

- Create `src/platform/time.h/.c`.
- Create `examples/sort_benchmark.c`.

### Concepts to understand

- monotonic versus wall-clock time;
- debug versus optimized builds;
- warmup and repeated measurements;
- benchmark inputs and compiler optimization.

### Required tests

- timestamps are representable and differences are nonnegative in ordinary use;
- duration conversion uses known synthetic values;
- benchmark correctness is checked before timing.

### Acceptance criteria

- benchmark records input size, data shape, build flags, and machine;
- insertion and merge sort outputs are checked for equality;
- conclusions distinguish asymptotic growth from one noisy timing.

### Optional hints

Generate inputs deterministically so comparisons can be reproduced.

## Milestone 22 — Memory mapping (optional platform milestone)

### Goal

Expose a read-only mapped file as a borrowed byte view.

### Files to create or change

- Create `src/platform/mapped_file.h/.c` for the current OS.
- Add integration tests.

### Concepts to understand

- virtual memory;
- page-backed mappings;
- OS handles;
- mapping lifetime;
- empty-file behavior.

### Required tests

- map and inspect a nonempty file;
- empty file policy;
- nonexistent file;
- cleanup;
- view becomes invalid after unmapping.

### Acceptance criteria

- mapping and file handles are released;
- lifetime restrictions are prominent;
- ordinary file reading remains available when ownership is preferable.

---

# Part VI — Trees and graph algorithms

These milestones reuse stacks, queues, heaps, vectors, and bitsets instead of building traversal infrastructure from scratch.

## Milestone 23 — Binary search tree

### Goal

Implement a concrete `i32` binary search tree.

### Files to create or change

- Create `src/collections/bst.h/.c`.
- Add tree tests.

### Concepts to understand

- recursive structure;
- ordering invariant;
- duplicate-value policy;
- insertion, lookup, traversal, and deletion;
- unbalanced worst case.

### Required tests

- empty tree;
- root, leaf, and several levels;
- duplicate policy;
- inorder traversal is sorted;
- delete leaf, one-child node, and two-child node;
- sorted insertion demonstrates degeneration;
- complete cleanup.

### Acceptance criteria

- every node satisfies the ordering policy;
- deletion preserves links and frees exactly one node;
- average versus worst-case complexity is explained.

### Optional hints

Implement lookup and insertion before deletion. Treat deletion as a separate sub-milestone.

## Milestone 24 — Graph representation

### Goal

Represent a directed graph using adjacency lists.

### Files to create or change

- Create `src/collections/graph.h/.c`.
- Add graph tests.

### Concepts to understand

- vertices and edges;
- adjacency lists;
- directed versus undirected graphs;
- weighted versus unweighted edges;
- graph ownership.

### Required tests

- zero vertices;
- isolated vertices;
- directed edge visibility;
- self-loop and duplicate-edge policy;
- invalid vertex indexes;
- growth and cleanup.

### Acceptance criteria

- vertex/edge ownership is explicit;
- adding an edge cannot leave a partially updated graph on failure;
- representation cost is explained as `O(V + E)`.

### Optional hints

Start unweighted and directed. Add edge weights only when Dijkstra's milestone begins.

## Milestone 25 — Breadth-first and depth-first search

### Goal

Traverse a graph and record visitation order or predecessor relationships.

### Files to create or change

- Create `src/algorithms/graph_search.h/.c`.
- Add traversal tests.

### Concepts to understand

- visited sets;
- queue frontier for BFS;
- stack/recursion frontier for DFS;
- disconnected components;
- `O(V + E)` traversal.

### Required tests

- empty graph policy;
- one vertex;
- chain, branch, cycle, and disconnected graph;
- no vertex visited twice;
- BFS shortest edge-count distances;
- deterministic order based on adjacency insertion order, if promised.

### Acceptance criteria

- BFS uses the queue and DFS uses a stack or documented recursion;
- cycles terminate;
- predecessor data can reconstruct reachable paths.

### Optional hints

Mark a BFS vertex visited when enqueuing it, not when dequeuing it, to avoid duplicate frontier entries.

## Milestone 26 — Topological sorting

### Goal

Order a directed acyclic graph so every edge points forward, and detect cycles.

### Files to create or change

- Add to `src/algorithms/graph_search.h/.c` or create a focused module.
- Add tests.

### Concepts to understand

- directed acyclic graphs;
- indegree;
- Kahn's algorithm or DFS finishing order;
- cycle detection.

### Required tests

- empty graph;
- single vertex;
- chain and branching DAG;
- multiple valid orders checked by edge constraints, not one exact sequence;
- self-cycle and multi-vertex cycle.

### Acceptance criteria

- every edge precedes its destination in successful output;
- cyclic input returns an explicit failure;
- algorithm complexity is `O(V + E)`.

### Optional hints

Tests should validate the ordering property because many DAGs have several correct answers.

## Milestone 27 — Dijkstra's shortest paths

### Goal

Find shortest paths from one source in a graph with nonnegative edge weights.

### Files to create or change

- Extend graph edges with weights deliberately.
- Create `src/algorithms/shortest_path.h/.c`.
- Adapt the heap to store distance/vertex entries or build a focused priority queue.
- Add tests.

### Concepts to understand

- relaxation;
- tentative distances;
- greedy selection;
- stale priority-queue entries;
- unreachable vertices;
- overflow in distance addition;
- nonnegative-weight requirement.

### Required tests

- one vertex;
- direct and multi-edge shorter paths;
- unreachable vertex;
- zero-weight edge;
- duplicate routes;
- stale heap entries;
- distance overflow handling;
- reject or assert negative weights according to the chosen type/API.

### Acceptance criteria

- distances and predecessors match known examples;
- unreachable distance has an explicit representation;
- overflow does not wrap to a falsely short path;
- limitations are documented.

### Optional hints

A decrease-key heap is not required initially; push the improved pair and ignore stale entries when popped.

---

# Part VII — Larger programs

Programs are checkpoints that reveal whether module APIs are genuinely useful.

## Milestone 28 — Word-frequency counter

### Goal

Read a text file, split it into words, count occurrences, and print results.

### Modules exercised

- file reading;
- string views;
- parsing/tokenization rules;
- hash map;
- vector and sorting.

### Required tests

- empty file;
- repeated words;
- punctuation and case policy;
- final word without newline;
- long word;
- binary/null-byte policy;
- deterministic output ordering;
- missing file.

### Acceptance criteria

- key lifetime is safe for the map's full use;
- tokenization policy is written down;
- counts are correct;
- output order is deterministic;
- all resources are cleaned up.

### Optional hints

Borrowing keys from the complete file buffer is safe only while that buffer remains alive and unchanged.

## Milestone 29 — Expression evaluator

### Goal

Tokenize and evaluate arithmetic expressions with parentheses and precedence.

### Modules exercised

- string views;
- integer parsing;
- stacks;
- error reporting;
- optionally an arena for syntax nodes.

### Required tests

- individual integer;
- each operator;
- precedence;
- parentheses;
- unary sign policy;
- whitespace;
- malformed expressions;
- division by zero;
- arithmetic overflow policy.

### Acceptance criteria

- tokenization and evaluation are separate stages;
- syntax errors identify a useful position;
- operator precedence is tested rather than assumed;
- runtime errors are not assertions.

### Optional hints

Start with two operators before implementing a full grammar.

## Milestone 30 — Pathfinding demo

### Goal

Load or construct a graph, request two vertices, and print a shortest path.

### Modules exercised

- graph;
- heap;
- Dijkstra;
- file/text parsing;
- vectors and formatting.

### Required tests

- reachable and unreachable destinations;
- source equals destination;
- malformed graph input;
- unknown vertex;
- known shortest path and total cost;
- deterministic tie policy if promised.

### Acceptance criteria

- program logic composes modules instead of reimplementing them;
- path reconstruction uses predecessor data;
- errors produce nonzero exit status and useful messages.

## Milestone 31 — Binary-file inspector

### Goal

Display offsets, bytes, and selected decoded integer values from a binary file.

### Modules exercised

- file reading or memory mapping;
- byte readers;
- formatting;
- string builder;
- CLI parsing.

### Required tests

- empty and short files;
- known byte pattern;
- final partial display row;
- endian decoding;
- nonexistent file;
- invalid option.

### Acceptance criteria

- output does not read beyond the file;
- offsets and bytes are formatted consistently;
- endian interpretation is explicit.

---

# Part VIII — Systems-programming branches

These come after the core curriculum. Complete them one at a time based on interest, not as prerequisites for earlier work.

## Milestone 32 — Logging

Build log levels, formatting, output destinations, and optional timestamps.

Learn variadic functions, `va_list`, formatting, global-state tradeoffs, and thread-safety concerns.

Tests should cover level filtering, long messages, formatting errors/policy, destination failures, and deterministic output with timestamps disabled or injected.

## Milestone 33 — Threads and mutexes

Build a small platform wrapper, then a bounded producer/consumer queue.

Learn data races, mutual exclusion, condition variables, spurious wakeups, shutdown, and ownership transfer between threads.

Tests should use deterministic coordination where possible and cover queue full/empty waiting, multiple producers/consumers, shutdown, and sanitizer tooling. Do not use timing sleeps as the primary correctness mechanism.

## Milestone 34 — Sockets

Build a small TCP echo client/server on the current platform.

Learn addresses, byte order, partial send/receive, connection lifecycle, blocking behavior, and protocol framing.

Tests should cover local loopback communication, empty and large messages, partial-operation loops, disconnects, malformed frames, and cleanup.

## Milestone 35 — Processes

Spawn a child process, capture output, inspect exit status, and handle errors.

Learn process creation, inherited resources, pipes, deadlocks from full pipes, and platform differences.

Tests should cover successful and failing commands, stdout/stderr capture, nonzero exit, missing executable, and cleanup.

## Milestone 36 — Second-language implementation

Reimplement selected stable modules in Zig or another language only after their C behavior is well understood.

Start with base types, vector, insertion sort, and tests. Compare safety model, allocation, error handling, generics, and foreign-function boundaries rather than translating syntax line by line.

---

# Entire project order at a glance

1. Insertion sort.
2. Merge sort.
3. Vector insertion/removal.
4. Number-sorting CLI.
5. Stack.
6. Ring-buffer queue.
7. Singly linked list.
8. Binary heap.
9. Concrete `i32` hash map.
10. Bitset.
11. Checked size arithmetic.
12. Fixed-buffer arena.
13. Allocation interface and deterministic failure injection.
14. Pool allocator.
15. String view.
16. String builder.
17. Integer parsing and formatting.
18. Byte reader/writer.
19. Whole-file I/O.
20. Directory traversal.
21. Timing and sorting benchmarks.
22. Optional memory mapping.
23. Binary search tree.
24. Graph representation.
25. BFS and DFS.
26. Topological sort.
27. Dijkstra's algorithm.
28. Word-frequency counter, including adaptation of the map for string-view keys.
29. Expression evaluator.
30. Pathfinding demo.
31. Binary-file inspector.
32. Optional logging.
33. Optional threads and mutexes.
34. Optional sockets.
35. Optional processes.
36. Optional second-language implementation.

This order is allowed to change when a real program exposes a better dependency order. The rule is to keep the next milestone concrete and small, not to predict every future abstraction perfectly.

---

# Current checkpoint

Work only on **Milestone 1: insertion sort** now.

The supplied tests intentionally fail because `src/algorithms/sort.h` does not exist. Your next three actions are:

1. Create and document `src/algorithms/sort.h`.
2. Create `src/algorithms/sort.c` and implement insertion sort.
3. Run `make test`, trace any failing case, and explain the sorted-prefix invariant and complexity.

After those pass, request review. The merge-sort tests and detailed implementation lesson should be activated only then.

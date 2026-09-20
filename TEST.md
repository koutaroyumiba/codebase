# AI test-generation guide

Use this file when asking an AI assistant to prepare tests for the current learning milestone.

## Role

Act as a test engineer supporting a learner. Automate verification without implementing the production code or revealing the solution.

The learner writes everything under `src/` and `programs/`. The AI may write or update tests and the minimum test/build infrastructure needed to run them.

## Before writing tests

1. Read `tmp.md` completely.
2. Identify the single milestone marked **active**.
3. Inspect its acceptance criteria and required-test list.
4. Read the relevant public headers, existing implementation, `Makefile`, and current tests.
5. Run the existing smallest relevant test command before changing anything.
6. Check `git status` so unrelated work is not overwritten.

If the public API is not specified clearly enough to compile a test, ask the learner to choose the contract. Do not silently design a large API or add production declarations merely to make tests compile.

## Scope rules

- Generate tests only for the active milestone.
- Do not add tests for future milestones.
- Do not write, complete, or repair production implementations.
- Do not weaken an existing test to accommodate incorrect code.
- Do not test private helpers or depend on implementation details.
- Make the smallest focused test change.
- Preserve unrelated learner changes.
- Do not add dependencies without permission.
- Do not modify generated files or `build/`.

A compile or link failure caused by a not-yet-created active-milestone API can be a valid initial **red** state. Existing unrelated tests should remain runnable whenever practical.

## What tests should verify

Prefer observable properties over exact internal behavior.

For each active API, consider:

1. **Boundary states** — empty, one element, first/last valid index, exact capacity.
2. **Representative behavior** — ordinary inputs that demonstrate the main contract.
3. **Adversarial shapes** — sorted/reversed values, duplicates, collisions, wraparound, cycles, or truncated data as relevant.
4. **Extreme values** — integer limits and allocation-size boundaries without invoking undefined behavior.
5. **Failure behavior** — malformed input, allocation failure, absent values, full buffers, and OS errors where relevant.
6. **State preservation** — a failed operation leaves the object in the documented valid state.
7. **Ownership and reuse** — cleanup, clear/reset, and reuse work as promised.
8. **Properties** — invariants such as ordering, element preservation, FIFO order, or parent/child relationships.

Tests must not intentionally invoke undefined behavior. Assertion preconditions normally need documentation and review rather than execution in the main test process; a failed assertion would terminate the entire runner unless a separate-process death-test facility exists.

## C test conventions

Follow the repository's C23 style and existing names:

- use `bool` test functions;
- return `true` on success and `false` on a failed check;
- use the existing `CHECK` mechanism unless test infrastructure is the active lesson;
- use `usize`, `i32`, and other project types consistently;
- name tests `test_<module>_<behavior>`;
- register every test with a descriptive display name;
- keep each test focused on one behavior category;
- use `const` for expected arrays and data that must not change;
- compare complete results, not just one convenient element;
- use deterministic data and fixed seeds;
- avoid sleeps, network access, current-time assumptions, and dependence on directory iteration order;
- release every resource created by a passing test.

Be careful with early-return checks: if a test allocates resources and `CHECK` returns immediately, a failed check can skip cleanup. For small stack-only tests this is harmless. For resource-owning tests, structure the test so cleanup always occurs, or propose a small cleanup-aware test helper before adding many such tests.

## Prefer property checks

Examples of useful properties:

- sorting: output is ordered **and** contains the same multiset of values;
- heap: each parent satisfies the heap relation and repeated pop has the expected order;
- queue: dequeue order equals enqueue order across wraparound and growth;
- map: every inserted key remains retrievable after collision and resize;
- arena: returned regions are aligned, non-overlapping, and inside the backing buffer;
- parser: successful formatting/parsing round trips preserve the value;
- graph algorithm: validate path/order constraints rather than demanding one answer when several are valid.

A reference implementation such as standard-library `qsort` may be used in tests when it provides an independent expected result. Do not copy the learner's algorithm into the test.

## Suggested test organization

### Keep the current structure initially

While the suite is small, keep tests in:

```text
tests/test_main.c
```

This minimizes build-system work while the learner focuses on algorithms and data structures.

### Split when navigation becomes painful

When `test_main.c` contains several unrelated modules or is roughly 500–700 lines, propose this structure:

```text
tests/
├── test.h                 shared CHECK macros and declarations
├── test_main.c            runner and suite registration
├── test_base.c
├── test_vector.c
├── test_sort.c
└── test_<module>.c
```

A module test file can expose one suite-registration function. Keep runner internals out of production headers. Make the Makefile discover or explicitly list test sources, but do not perform this refactor while an unrelated milestone is in progress unless the learner approves it.

For integration tests, use separate files when appropriate:

```text
tests/integration/
├── test_sort_numbers.sh
└── fixtures/
```

Use fixtures only when inline test data would be unreadable. Temporary files should be created during the test and removed afterward rather than committed as generated artifacts.

## Test quality checklist

Before finishing, verify:

- [ ] Tests correspond to the active milestone in `tmp.md`.
- [ ] Each acceptance criterion has automated coverage where practical.
- [ ] Tests compile against the documented public API.
- [ ] Tests do not depend on private representation unless the representation is itself the lesson.
- [ ] Failure messages identify the failing expression and location.
- [ ] Test data is deterministic.
- [ ] Resource cleanup is reliable.
- [ ] Existing tests were not removed or weakened.
- [ ] The expected initial failure is explained.
- [ ] The smallest relevant command was run.

## Response format after generating tests

Report only:

1. files changed;
2. behaviors covered;
3. the command run and its result;
4. why the current failure is expected, if the milestone is still red;
5. the learner's next implementation step from `tmp.md`.

Do not include a production implementation, direct solution, or a code snippet that effectively solves the milestone.

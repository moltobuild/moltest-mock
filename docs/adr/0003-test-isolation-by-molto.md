# 0003 Mocks replace real code through test isolation in molto — Status: Proposed
Date: 2026-10-04

## Context
ADR 0002 makes a mock the definition of the function, so the real one must not
be in the same test binary. It assumed "another library" would not be. Under
molto it always is: dependencies are compiled from source into the test binary
alongside every object of `src/`, so mocking anything real fails to link (KI-3).
One binary cannot hold both the real function and the mock; no macro in this
package can change that.

## Decision
Keep the fff-style macros of ADR 0002. Mocking real code is made possible by
molto (RFC-0021): a test file that declares what it replaces is linked into a
binary of its own, without those sources, while the rest of the suite keeps its
mode. This package documents the declaration, tests it end to end (e2e step 3
flips from "does not link" to "passes") and requires the molto version that
ships it.

## Alternatives considered
- **Opt-in weak symbols** (`MOLTEST_MOCKABLE`, weak only in test builds):
  covers calls inside one source file, but annotates production code and weak
  symbols are unreliable on MinGW, which CI covers (rejected in ADR 0002 too).
- **`--wrap` / objcopy weakening:** GNU-only or needs LLVM tools; not portable.
- **Document the limit and stop:** honest, but leaves the package usable only
  for functions nobody defines.

## Consequences
- The unit a test replaces is a source file: mocking one function of a `.c`
  means providing every function of it that the binary needs.
- A call between two functions of the same `.c` still cannot be mocked.
- Depends on molto shipping RFC-0021; until then KI-3 stays open.

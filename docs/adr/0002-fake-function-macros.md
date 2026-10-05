# 0002 fff-style fake function macros — Status: Accepted
Date: 2026-10-04

## Context
C has no reflection: a test cannot patch a function at run time as pytest's
`monkeypatch` does. The ways to replace one are a definition the linker picks
instead of the real one, link-time wrapping, or a function-pointer seam in the
code under test.

## Decision
Macros in the style of fff (Fake Function Framework):
`MOCK_VALUE_FUNC(ret, name, types...)` / `MOCK_VOID_FUNC(name, types...)`
define `name` itself plus a `name_mock` struct with `call_count`,
`argN_val`, `argN_history[]`, `history_dropped`, `return_val`, `return_seq`
and `custom_fake`. The test configures the struct and checks it with
moltest's ordinary EXPECT_*. `MOCK_DECLARE_*` / `MOCK_DEFINE_*` split a mock
between a header and one file. Arity is counted with C23 `__VA_OPT__`, 0 to 6.

## Alternatives considered
- **gmock-style expectations** (`EXPECT_CALL(f).with(x).times(1)`): more
  expressive, but needs matchers and a verification point, a much larger API in C.
- **`-Wl,--wrap=name`:** keeps the real function reachable, but GNU ld only;
  Apple ld and MSVC have no equivalent, and the CI runs on all three.
- **Weak symbols:** need the real function marked weak in production code.

## Consequences
- The real function must not be in the same test binary (link error otherwise):
  mocks fit calls across a boundary, not calls inside one object file.
- Uses `__VA_OPT__`, C23 but accepted by GCC and Clang in C11 and C17 too;
  the definitions end in `_Static_assert`, not C23's `static_assert`, so a
  default `molto new` project (C17) compiles them (KI-2). Under `-Wpedantic`
  before C23, a mock with no arguments warns. GCC and Clang verified in CI.
- Types with commas or declarators around the name need a typedef.

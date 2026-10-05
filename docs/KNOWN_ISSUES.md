# Known issues

## KI-1 GCC untested — Status: Resolved
- Repro: build and test with GCC (`-std=c2x -Wpedantic`); only Apple clang has run so far.
- Risk: `__VA_OPT__` and a variadic macro called with no variadic arguments
  (`MOCK_VALUE_FUNC(int, now)`) are C23; older GCC in `-std=c2x` may warn.
- Expected: no warnings on GCC 12+.
- Resolved: PR #1's CI, GCC on Linux and MSYS2: self-tests (C2x, `-Wpedantic`) and e2e (C17) build with no warnings.

## KI-2 Mocks did not compile in C17 — Status: Resolved
- Repro: `molto new` (C17 by default), add moltest-mock, define any mock.
- Expected: compiles. Actual: `static_assert` undeclared; it is a keyword only
  from C23, before that a macro of `<assert.h>`.
- Fix: the definitions end in `_Static_assert` in C (`static_assert` in C++).
- Test: `.github/e2e.sh` builds a default C17 `molto new` library with mocks.

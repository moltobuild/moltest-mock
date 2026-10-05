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

## KI-3 A function a real dependency defines cannot be mocked — Status: Open
- Repro: a `molto new` library with a `[deps]` entry that defines `f()`, and
  `MOCK_VALUE_FUNC(int, f)` in a test that calls code using `f()`.
- Expected: the mock replaces `f()` in that test, as the README promised for
  "another library". Actual: `duplicate symbol '_f'` (ld64, lld) or
  `multiple definition of 'f'` (GNU ld): molto compiles dependencies from source
  into the test binary, next to every object of `src/`. The same holds for a
  function of the project's own `src/`.
- What works today: only functions nobody in the binary defines. A libc
  function can be mocked, but the mock replaces it for the whole binary,
  moltest included (and, on Linux, libc's own internal calls).
- Test: `.github/e2e.sh` step 3 asserts the duplicate symbol; flip it when fixed.
- Fix: needs molto to link a test that mocks without the sources it replaces
  (molto RFC-0020, ADR 0003). Not fixable in this package alone.

# Known issues

## KI-1 GCC untested — Status: Open
- Repro: build and test with GCC (`-std=c2x -Wpedantic`); only Apple clang has run so far.
- Risk: `__VA_OPT__` and a variadic macro called with no variadic arguments
  (`MOCK_VALUE_FUNC(int, now)`) are C23; older GCC in `-std=c2x` may warn.
- Expected: no warnings on GCC 12+. To be closed by the M2 CI matrix.

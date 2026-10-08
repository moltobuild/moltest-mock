# Architecture

## Where it sits
```
molto test
 └─ <package>_tests
     ├─ moltest          runs the tests
     ├─ tests/*.c        MOCK_VALUE_FUNC(int, read_file, const char *)
     │                     defines read_file() and read_file_mock;
     │                     a constructor registers read_file_mock_reset
     └─ moltest-mock     reporter registered by a constructor (ADR 0001)
         on_run_start:   fail the run if mocks overflowed the registry
         on_test_start:  reset every mock (before BEFORE_EACH)
```

## Components
| Unit | Role |
|---|---|
| `include/moltest_mock.h` | the macros (ADR 0002); all a test file includes; does not need moltest.h |
| `src/registry.c` | fixed-size list of mock reset functions (pure, unit-tested) |
| `src/plugin.c` | the one registry, `moltest_mock_register`/`reset_all`, the reporter |
| `src/moltest_api.h` | copy of moltest's reporter API v1, checked by `tests/test_moltest_api.c` |

## Key decisions
- In-process plugin of moltest: [ADR 0001](adr/0001-in-process-plugin.md)
- fff-style fake function macros: [ADR 0002](adr/0002-fake-function-macros.md)
- Mocking real code through test isolation in molto: [ADR 0003](adr/0003-test-isolation-by-molto.md)

## Invariants
- A mock is a real definition of the function in the test binary; the real
  function must not be linked into the same binary. Under molto a test that
  mocks real code declares `[[test.isolated]]` and links without the sources it
  replaces (molto RFC-0021, [ADR 0003](adr/0003-test-isolation-by-molto.md)).
- Every mock starts each test at zero, before BEFORE_EACH.
- Every mock's constructor references `src/plugin.c`, so one mock brings the
  reporter in, whether the package is linked from source or as an archive.
- The struct layout depends on `MOLTEST_MOCK_HISTORY`: it is set for the whole
  binary, never per file.
- No dependency beyond moltest (consumer's own copy) and libc.

## Manifest package interface

Project.toml is the only carried description (Molto RFC-0024). The plugin
exports include/ by convention and declares moltest in [deps], since its
sources call that API. The graph shares one runner with consumers. Development
configuration is ignored when the plugin is consumed.

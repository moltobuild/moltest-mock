# moltest-mock

[![CI](https://github.com/moltobuild/moltest-mock/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/moltobuild/moltest-mock/actions/workflows/ci.yml)

Fake functions for C suites run by [moltest](https://github.com/moltobuild/moltest),
in the style of [fff](https://github.com/meekrosoft/fff). Requires moltest 0.3.0
or later and C11 or later (molto's default, C17, works).

```c
#include <moltest.h>
#include <moltest_mock.h>

MOCK_VALUE_FUNC(int, read_file, const char *);   /* defines read_file() */

DESCRIBE(loads_config) {
    read_file_mock.return_val = 0;
    load_config("x.toml");
    EXPECT_EQ(1, (int)read_file_mock.call_count);
    EXPECT_STREQ("x.toml", read_file_mock.arg0_val);
}
```

## Using it

```toml
[dev-deps]
moltest = { git = "https://github.com/moltobuild/moltest", tag = "v0.3.0" }
moltest_mock = { git = "https://github.com/moltobuild/moltest-mock", tag = "v0.1.0" }
```

Every mock is reset to zero before each test, before its `BEFORE_EACH`.

| Field | Meaning |
|---|---|
| `call_count` | calls so far |
| `argN_val` | argument N of the last call |
| `argN_history[i]` | argument N of call i (first `MOLTEST_MOCK_HISTORY`, default 50) |
| `history_dropped` | calls that did not fit in the history |
| `return_val` | what a call returns (zero by default) |
| `return_seq`, `return_seq_len` | one value per call, the last one repeated |
| `custom_fake` | a function with the same signature; wins over the above |

- `MOCK_VOID_FUNC(name, types...)` for functions returning `void`.
- `MOCK_DECLARE_VALUE_FUNC` in a header plus `MOCK_DEFINE_VALUE_FUNC` in one
  file to share a mock between test files (and the `VOID` pair).
- Up to 6 arguments; typedef function pointer and array types first.

The mock *is* the function, so the real one must not be linked into the same
test binary. Under molto today that rules out every function a `[deps]` entry
or your own `src/` defines: both are compiled into the test binary, and the
link fails with a duplicate symbol ([KI-3](docs/KNOWN_ISSUES.md)). What works
now is mocking functions nothing in the binary defines. Mocking real code needs
molto to link such a test without the sources it replaces (molto RFC-0020,
[ADR 0003](docs/adr/0003-test-isolation-by-molto.md)).

### Calls inside one source file

What a mock replaces is a call that crosses from one source file to another.
A call between two functions of the same `.c` cannot be mocked, with this
package or with fff: the linker keeps or drops a whole object, and the compiler
may resolve, or inline, a call within one file without the linker at all.

```c
/* src/config.c: load_config's call to read_file cannot be mocked */
int read_file(const char *path) { ... }
int load_config(const char *path) { return read_file(path) != 0 ? -1 : 0; }
```

Move `read_file` to `src/fs.c` and the call crosses a boundary: a test that
replaces `src/fs.c` (molto RFC-0020) can mock it and still test `load_config`.

Mocks are not thread-safe: code under test that calls one from several threads
gets counts and histories that race.

## License
Apache-2.0

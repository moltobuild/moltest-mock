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
test binary: mock what your code calls in another library or the system.

## License
Apache-2.0

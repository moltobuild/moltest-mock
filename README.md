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
moltest_mock = { git = "https://github.com/moltobuild/moltest-mock", tag = "v0.2.0" }
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

## Mocking real code

The mock *is* the function, so the real one must not be linked into the same
test binary. Under molto, `src/` and every `[deps]` entry are compiled into it,
so a test that mocks one of their functions says which source it replaces
(molto 0.52.0 or later, RFC-0021):

```toml
[[test.isolated]]
file     = "tests/test_config.c"
replaces = ["src/fs.c"]            # or "dep:src/x.c" for a dependency's source, or "dep"
```

That test links into an executable of its own without `src/fs.c`, and its
mocks stand in for what `src/fs.c` defined; the rest of the suite keeps the real
one. Without the declaration the link fails with a duplicate symbol.

The linker takes a file whole, from your sources and your dependencies alike.
So the test defines a mock for every function of a replaced source that the
files it links call, used or not. It also needs any mock it relied on from
another test file: it does not link those. A missing one is an undefined
symbol at the link ([ADR 0003](docs/adr/0003-test-isolation-by-molto.md)).

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
replaces `src/fs.c` (molto RFC-0021) can mock it and still test `load_config`.

Mocks are not thread-safe: code under test that calls one from several threads
gets counts and histories that race.

## Mocking through dependency injection

A mock that replaces a function at link time has the limits above. A mock
*passed in* has none: give the code under test its dependencies as a struct of
function pointers, and a test hands it mocks under names of their own. The real
functions stay in the binary, nothing collides, every test mode works today,
and a call inside one source file can be mocked too. It is what gmock does with
virtual interfaces, in C.

**1. Describe the dependency as an interface**, in a header of its own:

```c
/* include/fs_ops.h */
typedef struct {
    int (*read_file)(const char *path, char *out, size_t size);
    int (*write_file)(const char *path, const char *text);
} fs_ops;

extern const fs_ops fs_ops_real;   /* the implementation production uses */
```

**2. Implement it once** with the real functions:

```c
/* src/fs_ops.c */
static int real_read_file(const char *path, char *out, size_t size) { ... }
static int real_write_file(const char *path, const char *text) { ... }

const fs_ops fs_ops_real = {.read_file = real_read_file, .write_file = real_write_file};
```

**3. Take it as a parameter** in the code that uses it, never the functions
directly. Production passes `&fs_ops_real`:

```c
/* src/config.c */
int config_load(const fs_ops *fs, const char *path, config *out) {
    char text[4096];
    if(fs->read_file(path, text, sizeof text) != 0)
        return -1;
    ...
}
```

A component that needs several dependencies keeps them in one context struct,
built once at start-up, instead of a parameter per dependency.

**4. In the test, mock each function under its own name** and put the mocks in
the struct:

```c
/* tests/test_config.c */
#include <moltest.h>
#include <moltest_mock.h>
#include <config.h>

MOCK_VALUE_FUNC(int, fake_read_file, const char *, char *, size_t);
MOCK_VALUE_FUNC(int, fake_write_file, const char *, const char *);

static const fs_ops fake_fs = {.read_file = fake_read_file, .write_file = fake_write_file};

DESCRIBE(config_load_fails_when_the_file_cannot_be_read) {
    fake_read_file_mock.return_val = -1;
    config cfg;
    EXPECT_EQ(-1, config_load(&fake_fs, "app.toml", &cfg));
    EXPECT_STREQ("app.toml", fake_read_file_mock.arg0_val);
    EXPECT_EQ(0, (int)fake_write_file_mock.call_count);
}
```

`custom_fake` gives a mock behaviour, such as filling the buffer a real
`read_file` would. Mocks are reset before each test, so the struct can be shared
by every test of the file.

When to use which: injection when you design the code, or can change it;
link-time replacement for code you cannot change, such as a dependency calling
into another (molto RFC-0021). `tests/test_injection.c` runs this example.

## License
Apache-2.0

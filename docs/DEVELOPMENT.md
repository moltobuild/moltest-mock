# Development

```sh
molto build
molto test
molto fmt --check
molto lint
```

## Trying it on a project
```toml
[dev-deps]
moltest = { git = "https://github.com/moltobuild/moltest", tag = "v0.3.0" }
moltest_mock = { path = "../moltest-mock" }
```
Then, in a test file, `#include <moltest_mock.h>` and `MOCK_VALUE_FUNC(...)`
for a function the code under test calls but `src/` does not define.

## Releasing
Same as moltest-coverage, once the repo and CI exist (M2): one PR bumps
`version` in `Project.toml` and `recipe.toml`, then a `v*` tag on the merge.

## Conventions
Same as moltest: C23 (`-std=c2x`), molto preset for format and lint, public
macros `MOCK_*`, plumbing `MOLTEST_MOCK_*_`, functions `moltest_mock_*`,
Conventional Commits.

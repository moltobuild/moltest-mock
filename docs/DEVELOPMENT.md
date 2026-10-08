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
moltest = { git = "https://github.com/moltobuild/moltest", tag = "v0.4.0" }
moltest_mock = { path = "../moltest-mock" }
```
Then, in a test file, `#include <moltest_mock.h>` and `MOCK_VALUE_FUNC(...)`
for a function the code under test calls but `src/` does not define.

## Releasing
1. One PR bumps `version` in `Project.toml`;
   `.github/check-version.sh <version>` checks both.
2. After it merges, tag the merge commit and push the tag:
   `git tag -a v0.1.0 -m "moltest-mock 0.1.0" && git push origin v0.1.0`.
3. `.github/workflows/release.yml` checks the tag against both, runs the CI on
   three platforms, and publishes the GitHub Release. Consumers pin it with
   `tag = "v0.1.0"`. Running the workflow by hand rehearses it without publishing.

## Conventions
Same as moltest: C23 (`-std=c2x`), molto preset for format and lint, public
macros `MOCK_*`, plumbing `MOLTEST_MOCK_*_`, functions `moltest_mock_*`,
Conventional Commits.

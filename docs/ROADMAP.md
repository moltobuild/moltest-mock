# Roadmap

## M0 - Repository and design
- [x] Repository, manifest, recipe, docs scaffold
- [x] ADR 0001 in-process plugin, ADR 0002 fff-style fake function macros
- [x] Spec 001 (MVP fakes)

## M1 - MVP (spec 001)
- [x] `MOCK_VALUE_FUNC` / `MOCK_VOID_FUNC`, 0 to 6 arguments
- [x] Call count, last arguments, argument history, dropped-history count
- [x] `return_val`, `return_seq`, `custom_fake`
- [x] `MOCK_DECLARE_*` / `MOCK_DEFINE_*` to share a mock across test files
- [x] Every mock reset before each test (plugin on `on_test_start`)
- [x] Self-tests, `molto fmt --check` and `molto lint` clean

## M2 - Ready to publish
- [x] GitHub repo `moltobuild/moltest-mock`
- [x] CI on Linux (gcc), macOS (clang), Windows (MSYS2 gcc), as moltest-coverage's (KI-1), molto 0.49.0
- [x] E2E: a default (C17) `molto new` library that mocks two functions of another library
- [x] Release by tag (release.yml, `.github/check-version.sh`)
- [x] CI green on all three platforms; KI-1 closed
- [ ] Release 0.1.0 (tag v0.1.0)

## Non-goals
- gmock-style expectations declared before the call (ADR 0002).
- Link-time interposition (`--wrap`, weak symbols): not portable (ADR 0002).
- Mocking variadic functions, or a function defined in the same binary as the mock.
- Runtime dependencies beyond moltest and libc.

## Backlog
- Global call order across mocks (`moltest_mock_calls`, "a called before b")
- Assertion helpers (`EXPECT_CALLED`, `EXPECT_CALLED_WITH`) on top of moltest's EXPECT_*
- Automatic verification at test end: needs a pre-verdict hook in moltest (moltest ADR, then here)
- C++: compile-tested header, `extern "C"` mocks from C++ tests
- `MOCK_VALUE_FUNC(int, now, void)` so a no-argument mock is `-Wpedantic` clean before C23
- More than 6 arguments

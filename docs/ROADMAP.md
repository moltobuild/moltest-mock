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
- [x] Release 0.1.0 (tag v0.1.0)

## M3 - Mock real code (KI-3, ADR 0003)
- [x] KI-3 logged; e2e step 3 reproduces it with a real `[deps]` entry
- [x] README and ARCHITECTURE say what works today
- [x] README guide: mocking through dependency injection, run by `tests/test_injection.c`
- [x] Release 0.2.0 (tag v0.2.0): KI-3 documented, injection guide
- [ ] molto RFC-0021 accepted and released
- [ ] Spec 002: declaring what a test replaces, e2e step 3 passes
- [ ] ADR 0003 Accepted, KI-3 resolved, release 0.3.0

## Non-goals
- gmock-style expectations declared before the call (ADR 0002).
- Link-time interposition (`--wrap`, weak symbols): not portable (ADR 0002).
- Mocking variadic functions, or a call between two functions of one source file.
- Runtime dependencies beyond moltest and libc.

## Backlog
- Global call order across mocks (`moltest_mock_calls`, "a called before b")
- Assertion helpers (`EXPECT_CALLED`, `EXPECT_CALLED_WITH`) on top of moltest's EXPECT_*
- Automatic verification at test end: needs a pre-verdict hook in moltest (moltest ADR, then here)
- C++: compile-tested header, `extern "C"` mocks from C++ tests
- `MOCK_VALUE_FUNC(int, now, void)` so a no-argument mock is `-Wpedantic` clean before C23
- More than 6 arguments
- Thread-safe call recording (atomics or a lock per mock)

# Validation

## Commands
| Check | Command |
|---|---|
| Build | `molto build` |
| Tests | `molto test` |
| Format | `molto fmt --check` |
| Lint | `molto lint` |

## CI
`.github/workflows/ci.yml`, molto pinned in `MOLTO_VERSION` (0.49.0),
installed by `.github/install-molto.sh`:

| Job | Runs on | Checks |
|---|---|---|
| Test | Linux (gcc), macOS (clang), Windows (MSYS2 gcc) | `molto build`, `molto test` |
| E2E | the same three | `.github/e2e.sh`: a default C17 `molto new` library with mocks passes; a wrong expectation fails; mocking a real `[deps]` function still does not link (KI-3) |
| Style | Linux, LLVM 19 | `molto fmt --check`, `molto lint` (a gate) |

`.github/workflows/release.yml` runs on a `v*` tag: the tag must equal the
version in Project.toml and recipe.toml, the CI above runs again, and only then
is the GitHub Release published.

## Strategy
- The macros are tested by using them: each spec criterion is a `DESCRIBE`
  in `tests/` that mocks a function and calls it.
- The per-test reset is tested so that it fails whatever order tests run in
  (`tests/test_reset.c`); removing the reset makes 9 assertions fail.
- The registry is pure and tested at its bound.
- Every acceptance criterion names its test.

## Definition of Done
- [ ] All acceptance criteria have passing tests
- [ ] Build, tests, format and lint pass
- [ ] SECURITY.md checklist reviewed
- [ ] Spec, ROADMAP and PLAN updated
- [ ] PROGRESS entry, commit, graphs refreshed

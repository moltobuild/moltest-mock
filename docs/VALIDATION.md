# Validation

## Commands
| Check | Command |
|---|---|
| Build | `molto build` |
| Tests | `molto test` |
| Format | `molto fmt --check` |
| Lint | `molto lint` |

## CI
Not yet (M2): the plan is moltest-coverage's matrix, Linux gcc, macOS clang,
Windows MSYS2 gcc, plus a Style gate.

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

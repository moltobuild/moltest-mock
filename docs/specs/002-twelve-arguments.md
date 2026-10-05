# 002 Mocks of up to twelve arguments — Status: Done
## Goal
Mock a function of up to twelve arguments with the macros, instead of six.
Found in molto: `source_fetch` takes eight, and its fake had to be written by
hand. Twelve leaves a wide margin over any real API seen so far.

## Acceptance criteria
- [x] AC1: a value mock of 7 to 12 arguments records each argument (`argN_val`, `argN_history`) and returns `return_val` → test: value_mocks_take_up_to_twelve_arguments
- [x] AC2: a void mock of 12 arguments records them and passes all twelve to `custom_fake` → test: void_mocks_take_twelve_arguments_into_custom_fake
- [x] AC3: 0 to 6 arguments behave exactly as before → test: mocks_take_zero_to_six_arguments (unchanged)
- [x] AC4: `MOLTEST_MOCK_ARGS_MAX` is 12 → test: value_mocks_take_up_to_twelve_arguments

## Design
ADR 0002 unchanged: the per-arity helpers (`PARAMS_n`, `TYPES_n`, `ARGS_n`,
`FIELDS_n`, `SAVE_n`) gain 7 to 12, and the argument counter counts to 12.

## Security notes
None: more fields of the same bounded kind, the history still capped by
`MOLTEST_MOCK_HISTORY`.

## Tasks
- [x] Counter and per-arity helpers up to 12
- [x] Tests for AC1, AC2
- [x] README, ADR 0002, ROADMAP, spec 001 note

## Out of scope
A thirteenth argument is a compile error, as a seventh was before; not tested
in the suite, which would not build.

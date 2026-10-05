# 001 MVP fakes — Status: Done
## Goal
Define a fake for a C function in one line, configure what it returns, check
how it was called, with every mock reset before each test.

## Acceptance criteria
- [x] AC1: `MOCK_VALUE_FUNC` defines the function; it returns zero, or `return_val` once set → test: returns_zero_until_a_test_says_otherwise, returns_return_val
- [x] AC2: `call_count`, the last arguments (`argN_val`) and each call's (`argN_history`) are recorded → test: records_the_arguments_of_every_call
- [x] AC3: calls past `MOLTEST_MOCK_HISTORY` are counted, update `argN_val`, and add to `history_dropped`, without writing past the history → test: calls_past_the_history_are_counted_but_not_kept
- [x] AC4: `return_seq` gives one value per call, then repeats its last → test: return_seq_gives_one_value_per_call_then_repeats_the_last
- [x] AC5: `custom_fake` gets the arguments and wins over `return_seq` and `return_val` → test: custom_fake_wins_over_return_values
- [x] AC6: `MOCK_VOID_FUNC` records calls and calls `custom_fake` → test: void_mock_records_and_calls_custom_fake
- [x] AC7: 0 to 6 arguments → test: mocks_take_zero_to_six_arguments
- [x] AC8: every mock is reset before each test, before BEFORE_EACH; `name_mock_reset()` and `moltest_mock_reset_all()` reset by hand → test: state_from_another_test_is_gone_a/_b, before_each_runs_after_the_reset, a_test_can_reset_one_mock_or_all_of_them
- [x] AC9: `MOCK_DECLARE_*` in a header and `MOCK_DEFINE_*` in one file share a mock between test files → test: a_shared_mock_works_where_it_is_defined, a_shared_mock_works_where_it_is_only_declared
- [x] AC10: the registry resets every mock added and refuses past `MOLTEST_MOCK_MAX`, counting the refusals (the plugin then fails the run) → test: registry_resets_every_mock_added, registry_refuses_mocks_past_the_limit
- [x] AC11: `src/moltest_api.h` matches moltest.h field by field → test: the_api_copy_matches_moltest

## Design
ADR 0001 (plugin), ADR 0002 (macros); ARCHITECTURE.

## Security notes
Bounded history and registry, pointers stored never dereferenced (SECURITY.md).

## Tasks
- [x] Header macros (value, void, declare/define)
- [x] Registry and plugin
- [x] Self-tests for AC1-AC11
- [x] fmt and lint clean

## Out of scope
Expectations, call order across mocks, C++ tests, CI (M2).

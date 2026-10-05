# 0001 An in-process plugin of moltest — Status: Accepted
Date: 2026-10-04

## Context
moltest lists mocking frameworks among its non-goals ("plugins may do this").
Fakes need per-test isolation: a return value or a call count left by one test
must not reach the next. moltest's plugin API v1 (its ADR 0002, spec 004) lets
a linked package register a reporter that sees every test start.

## Decision
moltest-mock is a separate molto package (static library, consumed from
source) added to `[dev-deps]`, like moltest-coverage. A header holds the macros;
`src/` keeps a registry of every mock and registers a moltest reporter that
resets them all in `on_test_start`, which runs before BEFORE_EACH. The reporter
API is declared in `src/moltest_api.h` and checked against moltest.h by a test.

## Alternatives considered
- **Inside moltest:** rejected by moltest's non-goals; it would grow the header
  every suite includes.
- **Header-only, reset by hand:** no `src/`, but every test file would need a
  BEFORE_EACH listing its mocks, and a forgotten one leaks state silently.
- **Automatic verification at test end:** needs a hook before moltest decides a
  test's status, which API v1 lacks; deferred (Backlog, would need a moltest ADR).

## Consequences
- Requires moltest 0.3.0 or later (reporter API v1).
- Linking one mock is the whole setup; the reporter comes with it.
- A registry overflow fails the run (`moltest_fail_run`) rather than leaking state.

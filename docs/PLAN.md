# moltest-mock
Fake functions for C suites run by moltest, in the style of fff. One macro
defines a stand-in for a function the code under test calls across a boundary
(another library, the system); the test sets what it returns and checks how it
was called. A plugin of moltest: added as a development dependency, it resets
every mock before each test. moltest itself keeps mocking out of its core
(its ROADMAP non-goals).

## Current focus
Milestone: M4 - Twelve arguments · Spec: specs/002-twelve-arguments.md (done) · Next step: release 0.4.0

## Docs
[ROADMAP](ROADMAP.md) · [ARCHITECTURE](ARCHITECTURE.md) · [SECURITY](SECURITY.md) ·
[VALIDATION](VALIDATION.md) · [DEVELOPMENT](DEVELOPMENT.md) ·
[DEPENDENCIES](DEPENDENCIES.md) · [PROGRESS](PROGRESS.md) ·
[KNOWN_ISSUES](KNOWN_ISSUES.md) · [specs/](specs/) · [adr/](adr/)

## Package migration
Current task: RFC-0024 ([spec](specs/003-manifest-package.md)); remove the recipe and validate the manifest consumer interface.

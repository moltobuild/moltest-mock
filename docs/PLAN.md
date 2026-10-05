# moltest-mock
Fake functions for C suites run by moltest, in the style of fff. One macro
defines a stand-in for a function the code under test calls across a boundary
(another library, the system); the test sets what it returns and checks how it
was called. A plugin of moltest: added as a development dependency, it resets
every mock before each test. moltest itself keeps mocking out of its core
(its ROADMAP non-goals).

## Current focus
Milestone: M2 - Ready to publish (done, v0.1.0 released) · Next step: pick the next milestone from the Backlog

## Docs
[ROADMAP](ROADMAP.md) · [ARCHITECTURE](ARCHITECTURE.md) · [SECURITY](SECURITY.md) ·
[VALIDATION](VALIDATION.md) · [DEVELOPMENT](DEVELOPMENT.md) ·
[DEPENDENCIES](DEPENDENCIES.md) · [PROGRESS](PROGRESS.md) ·
[KNOWN_ISSUES](KNOWN_ISSUES.md) · [specs/](specs/) · [adr/](adr/)

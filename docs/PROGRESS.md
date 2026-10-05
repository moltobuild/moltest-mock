# Progress

## 2026-10-04 — Repository and spec 001 (MVP fakes)
- Done: repo scaffold mirroring moltest-coverage, ADR 0001 and 0002, spec 001;
  `MOCK_VALUE_FUNC`/`MOCK_VOID_FUNC`, declare/define split, per-test reset plugin
- Commit: see git log
- Tests: 17 passed (566 assertions) on Apple clang; fmt and lint clean
- Next: M2, CI matrix (KI-1 GCC), GitHub repo, release 0.1.0

## 2026-10-04 — M2 CI, e2e and KI-2
- Done: CI matrix (Linux gcc, macOS clang, Windows MSYS2), e2e on a default
  C17 `molto new` library, release by tag; KI-2 fixed (mocks did not compile in C17)
- Commit: 1c76238 (fix), see git log for CI
- Tests: 17 self-tests pass; e2e passes locally and fails on the pre-fix header
- CI: all green on Linux, macOS, Windows; GCC without warnings (KI-1 closed)
- Next: merge PR #1, tag v0.1.0

## 2026-10-05 — M2 done, v0.1.0 released
- Done: PR #1 merged (62641d4); tag v0.1.0, Release workflow green, GitHub Release published
- Tests: CI green on Linux, macOS, Windows; no open known issues
- Next: choose the next milestone from the Backlog

## 2026-10-04 — M3 start: KI-3
- Done: found that a function a real `[deps]` entry (or `src/`) defines cannot be
  mocked under molto; KI-3 logged, e2e step 3 reproduces it, README and
  ARCHITECTURE corrected, ADR 0003 proposed, M3 added to ROADMAP
- Tests: 17 self-tests pass; e2e passes, step 3 asserts the duplicate symbol
- Next: molto RFC-0020 (isolated test binaries for mocking tests)

# Security

## Threat model
Test-only code: it runs inside the consumer's test binary with the developer's
or CI's privileges and is never linked into shipped code. It reads no files,
environment or network; its inputs are the arguments the code under test
passes to a mock.

## Rules
- Fixed-size storage only: argument history is capped (`MOLTEST_MOCK_HISTORY`),
  the registry too (`MOLTEST_MOCK_MAX`); overflow is counted, never written past.
- Arguments are stored by value; pointers are kept, never dereferenced.
- A full registry fails the run instead of leaking state between tests.

## Per-feature checklist
- [ ] Every buffer has a bound and a test at the bound
- [ ] No dereference of caller pointers inside the library
- [ ] No I/O, no environment, no shell

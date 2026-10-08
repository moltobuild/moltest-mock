# Dependencies

## Policy
Same as moltest (decided 2026-10-04):
- Exact versions only; no install or post-install scripts.
- Zero runtime dependencies beyond moltest and libc.
- Any new dependency needs an ADR and the maintainer's approval.
- `Molto.lock` is committed whenever one exists.

## Runtime
| Name | Why |
|---|---|
| moltest | the runner this plugs into (consumer's own copy, API declared in `src/moltest_api.h`) |

## Development
| Name | Version | Why |
|---|---|---|
| moltest | v0.3.0 (tag) | runs the self-tests; reporter API v1 |

## External tools
| Tool | Why |
|---|---|
| molto | build, test, fmt, lint |

The RFC-0024 migration pins moltest to `9e0611007d3b1ccebd589273dd14a7a229265c3c` in [deps] until
a compatible release tag is published. Consumers must select the same revision.

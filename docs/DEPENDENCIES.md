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
| moltest | v0.4.0 (tag) | runs the self-tests; reporter API v1 |

## External tools
| Tool | Why |
|---|---|
| molto | build, test, fmt, lint |

The manifest pins moltest to the released `v0.4.0` tag in `[deps]`.
Consumers must select the same tag to share one runner with this plugin.

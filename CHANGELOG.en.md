# Changelog

中文: [CHANGELOG.md](CHANGELOG.md)

Fork maintenance changes only; for product changes see upstream releases.

## [Unreleased]

- Traditional Chinese `README.md`, maintenance docs, Windows maintenance gate, upstream tracking and divergence-registry checks, matching CI.
- Only the `main` branch and the latest upstream version tag/release are kept (currently `v0.12.1`, assets mirrored from upstream).
- Triage record of open upstream issues/PRs.
- Fixes for Windows-relevant defects with regression tests (cypher overflow and resource exhaustion, watcher on non-ASCII paths and unplugged drives, `cbm_setenv`, `cbm_readdir`, HTTP UI, like hints, installer verification, and more); see `REVIEW.md`.
- Second review pass: store, route data flows, extractors, TOML multi-line arrays, file-size cap, allocation checks; see `REVIEW.md`.
- Synced upstream to `e71f23e` (2026-10-07) with an incremental triage record of new PRs/issues.
- Third review pass (extractors and every language LSP): fixes for 58 reproduced hangs, crashes and wrong results; see `REVIEW.md`.
- Synced upstream to `ec6427c` (v0.12.0, 2026-10-16) with an incremental triage record; re-enabled the `test-impact-artifact` workflow.
- Synced upstream to `a7158ac` (v0.12.1) and `10ab613` (2026-10-24, 294 commits) with an incremental triage record; the watcher UTF-8 path handling and Rust generic impls now use the upstream version; the TS LSP walk cap is halved (512 levels exhausted the 8 MiB stack under ASan).
- Windows fix: a file named `nul` in a repository aborted every index run (upstream issue #2646).

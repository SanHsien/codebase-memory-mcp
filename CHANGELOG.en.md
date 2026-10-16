# Changelog

中文: [CHANGELOG.md](CHANGELOG.md)

Fork maintenance changes only; for product changes see upstream releases.

## [Unreleased]

- Traditional Chinese `README.md`, maintenance docs, Windows maintenance gate, upstream tracking and divergence-registry checks, matching CI.
- Only the `main` branch and the `v0.11.0` tag/release are kept.
- Triage record of open upstream issues/PRs.
- Fixes for Windows-relevant defects with regression tests (cypher overflow and resource exhaustion, watcher on non-ASCII paths and unplugged drives, `cbm_setenv`, `cbm_readdir`, HTTP UI, like hints, installer verification, and more); see `REVIEW.md`.
- Second review pass: store, route data flows, extractors, TOML multi-line arrays, file-size cap, allocation checks; see `REVIEW.md`.
- Synced upstream to `e71f23e` (2026-10-07) with an incremental triage record of new PRs/issues.
- Third review pass (extractors and every language LSP): fixes for 58 reproduced hangs, crashes and wrong results; see `REVIEW.md`.
- Synced upstream to `ec6427c` (v0.12.0, 2026-10-16) with an incremental triage record; re-enabled the `test-impact-artifact` workflow.
- Windows fix: a file named `nul` in a repository aborted every index run (upstream issue #2646).

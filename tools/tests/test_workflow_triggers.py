"""契約測試：分岔登記檢查必須在每次推送 main 時執行。"""

from __future__ import annotations

from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]


def test_fork_ci_runs_the_gate_on_every_push_to_main() -> None:
    # tools/dev_check.ps1 runs check_divergence.py, whose whole job is to catch an
    # edit to an upstream-owned file (src/, internal/, vendored/) that docs/DIVERGENCE.md
    # does not register. Filtering those directories out of the push trigger means a
    # C-only commit with no registry row reaches main with no check at all, and is only
    # noticed by the weekly upstream-check schedule.
    workflow = (REPO_ROOT / ".github" / "workflows" / "fork-ci.yml").read_text(encoding="utf-8")
    assert "paths-ignore" not in workflow
    assert "paths:" not in workflow

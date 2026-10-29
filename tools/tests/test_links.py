"""契約測試：維護文件的相對連結要以 git 追蹤的路徑（大小寫完全一致）為準。"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import check_links as checker  # noqa: E402

AGENTS = checker.ROOT / "AGENTS.md"


def test_a_link_to_a_tracked_file_and_directory_resolves() -> None:
    assert checker._missing_relative(AGENTS, "docs/DEVELOPMENT.md") is None
    assert checker._missing_relative(AGENTS, "docs/DEVELOPMENT.md#驗證") is None
    assert checker._missing_relative(AGENTS, "docs/") is None


def test_a_link_whose_case_differs_from_the_tracked_file_is_broken() -> None:
    # Windows and macOS file systems ignore case, so `exists()` finds the file and the
    # gate passes; GitHub serves paths case-sensitively and the link is a 404 there.
    assert checker._missing_relative(AGENTS, "docs/development.md") is not None


def test_a_link_to_an_untracked_local_file_is_broken(
    tmp_path: Path, monkeypatch: pytest.MonkeyPatch
) -> None:
    # A file that exists only in this checkout (build output, an ignored report) is
    # not on GitHub either.
    # Clean up any legacy remnant in the repo if a previous test run was abruptly killed.
    legacy = checker.ROOT / "tools" / "tests" / "_untracked_link_target.md"
    if legacy.exists():
        legacy.unlink()

    # The temporary target is placed outside the repo in tmp_path so abrupt test aborts
    # never dirty the repository.
    subprocess.run(["git", "init", "-q"], cwd=tmp_path, check=True)
    doc = tmp_path / "AGENTS.md"
    doc.write_text("# Test\n", encoding="utf-8")
    untracked = tmp_path / "tools" / "tests" / "_untracked_link_target.md"
    untracked.parent.mkdir(parents=True, exist_ok=True)
    untracked.write_text("local only\n", encoding="utf-8")

    monkeypatch.setattr(checker, "ROOT", tmp_path)
    checker._tracked_paths.cache_clear()
    try:
        assert (
            checker._missing_relative(doc, "tools/tests/_untracked_link_target.md")
            is not None
        )
    finally:
        checker._tracked_paths.cache_clear()

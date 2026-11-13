"""Contract test: huge generated sources must stay out of the test-impact graph.

The `test-impact-artifact` workflow builds the whole code graph of HEAD from
scratch whenever no previous bundle exists, under a hard 30-minute deadline
(`TIC_DEFAULT_DEADLINE_S` in src/mcp/test_impact_cli.c). The inventory walks every
tracked file that discovery does not skip, and discovery only skips directories
named `vendored`/`vendor` and whatever `.gitignore` / `.cbmignore` exclude. The
generated tree-sitter `parser.c` files under `tools/tree-sitter-*/src/` (40 MB each
for the SQL and VB.NET grammars) are not in a skipped directory, so adding two of
them grew the tracked C bytes of `tools/` from 26 MB to 109 MB and pushed the
full-graph build past the deadline: "reading HEAD sources failed: internal/cbm/ac.c".
"""

from __future__ import annotations

import subprocess
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]

C_FAMILY = (".c", ".h", ".cc", ".cpp", ".hpp", ".m", ".mm", ".cxx", ".hh")
# Hand-written C in this tree tops out at 1.6 MB (tests/test_pipeline.c); the generated
# parsers under tools/ run from 2.8 MB (javascript) to 42 MB (sql).
GENERATED_SIZE_BYTES = 2 * 1024 * 1024
# Discovery never enters these directories, so files below them cost nothing.
SKIPPED_DIRS = {"vendored", "vendor", "node_modules", "build"}


def _git(*args: str) -> str:
    return subprocess.run(
        ["git", "-C", str(REPO_ROOT), *args],
        check=True,
        capture_output=True,
        text=True,
        encoding="utf-8",
    ).stdout


def _tracked_large_c_sources() -> list[str]:
    found = []
    for rel in _git("ls-files", "-z").split("\0"):
        if not rel or not rel.endswith(C_FAMILY):
            continue
        if SKIPPED_DIRS.intersection(rel.split("/")[:-1]):
            continue
        path = REPO_ROOT / rel
        if path.is_file() and path.stat().st_size > GENERATED_SIZE_BYTES:
            found.append(rel)
    return found


def _ignored_by_cbmignore(paths: list[str]) -> set[str]:
    # `.cbmignore` is gitignore syntax (docs/cbmignore.md); git's own matcher, pointed at
    # that file with --no-index (the paths are tracked), answers the same question.
    cbmignore = REPO_ROOT / ".cbmignore"
    assert cbmignore.is_file(), ".cbmignore is missing"
    result = subprocess.run(
        [
            "git",
            "-C",
            str(REPO_ROOT),
            "-c",
            f"core.excludesFile={cbmignore.as_posix()}",
            "check-ignore",
            "--no-index",
            "-z",
            "--stdin",
        ],
        input="\0".join(paths) + "\0",
        capture_output=True,
        text=True,
        encoding="utf-8",
    )
    assert result.returncode in (0, 1), result.stderr
    return {p for p in result.stdout.split("\0") if p}


def test_generated_parsers_under_tools_are_excluded_from_the_graph() -> None:
    large = _tracked_large_c_sources()
    assert any(p.startswith("tools/tree-sitter-") for p in large), (
        "no large tools/tree-sitter-* parser found: the size threshold or layout changed"
    )
    ignored = _ignored_by_cbmignore(large)
    leaked = sorted(set(large) - ignored)
    assert not leaked, (
        "tracked C-family sources over 2 MiB reach the test-impact graph build "
        "(add them to .cbmignore, see this file's docstring): " + ", ".join(leaked)
    )

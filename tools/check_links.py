#!/usr/bin/env python3
"""檢查本 fork 維護文件之間的相對連結。

只驗公開入口與 docs。外部網址交給人看。

    python tools/check_links.py
"""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path
from urllib.parse import unquote

ROOT = Path(__file__).resolve().parent.parent
LINK_PATTERN = re.compile(r"\[[^\]]*\]\(([^)]+)\)")
IMAGE_PATTERN = re.compile(r"!\[[^\]]*\]\(([^)]+)\)")
HTML_SRC_PATTERN = re.compile(r"""(?is)<img[^>]+src=["']([^"']+)["']""")
SKIP_PREFIXES = ("http://", "https://", "mailto:", "tel:", "#")
SKIP_NAMES = {
    "upstream-review-report.md",
    "dependency-freshness-report.md",
}
MAINTAINED_DOCUMENTS = (
    "README.md",
    "FORK.md",
    "NOTICE.md",
    "AGENTS.md",
    "CLAUDE.md",
    "GEMINI.md",
    "REVIEW.md",
    "CHANGELOG.md",
    "docs/DEVELOPMENT.md",
    "docs/DECISIONS.md",
    "docs/DIVERGENCE.md",
    "docs/UPSTREAM.md",
    "docs/records/upstream-triage-2026-09-29.md",
)


def _tracked(relative: str) -> bool:
    """Is the file in git's index?  A gitignored file exists locally but not in CI."""
    result = subprocess.run(
        ["git", "ls-files", "--error-unmatch", relative],
        cwd=ROOT,
        capture_output=True,
        check=False,
    )
    return result.returncode == 0


def iter_documents() -> list[Path]:
    missing = [
        relative
        for relative in MAINTAINED_DOCUMENTS
        if not (ROOT / relative).is_file() or not _tracked(relative)
    ]
    if missing:
        raise FileNotFoundError("maintained documents missing: " + ", ".join(missing))
    required = [ROOT / relative for relative in MAINTAINED_DOCUMENTS]
    return required


def _missing_relative(path: Path, target: str) -> str | None:
    target = target.strip().strip("<>")
    if not target or target.startswith(SKIP_PREFIXES):
        return None
    file_part = unquote(target.split("#", 1)[0])
    if not file_part:
        return None
    resolved = (path.parent / file_part).resolve()
    if resolved.exists():
        return None
    try:
        shown = resolved.relative_to(ROOT)
    except ValueError:
        shown = resolved
    return f"{target} → 找不到 {shown}"


def check_document(path: Path) -> list[str]:
    problems: list[str] = []
    text = path.read_text(encoding="utf-8")
    for pattern in (LINK_PATTERN, IMAGE_PATTERN, HTML_SRC_PATTERN):
        for match in pattern.finditer(text):
            missing = _missing_relative(path, match.group(1))
            if missing:
                problems.append(missing)
    return problems


def main() -> int:
    documents = iter_documents()
    if not documents:
        print("找不到任何維護用 Markdown 檔")
        return 1

    failures = 0
    for path in documents:
        problems = check_document(path)
        rel = path.relative_to(ROOT)
        if problems:
            failures += 1
            for problem in problems:
                print(f"FAIL {rel}: {problem}")
        else:
            print(f"OK   {rel}")

    print(f"\n共 {len(documents)} 份文件，{failures} 份有斷掉的相對連結。")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())

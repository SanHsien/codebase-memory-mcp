"""scripts/setup.sh must verify the release archive against checksums.txt before installing."""

import hashlib
import re
import shutil
import subprocess
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
SETUP = ROOT / "scripts" / "setup.sh"
ASSET = "codebase-memory-mcp-linux-amd64.tar.gz"
BASH = shutil.which("bash")


def _function_source() -> str:
    text = SETUP.read_text(encoding="utf-8")
    match = re.search(r"^verify_release_checksum\(\) \{\n.*?^\}\n", text, re.M | re.S)
    assert match, "setup.sh has no verify_release_checksum()"
    return match.group(0)


def _verify(tmp_path: Path, sums_text: str, archive_bytes: bytes = b"archive") -> int:
    archive = tmp_path / ASSET
    archive.write_bytes(archive_bytes)
    sums = tmp_path / "checksums.txt"
    sums.write_text(sums_text, encoding="utf-8", newline="\n")
    script = tmp_path / "check.sh"
    script.write_text(
        _function_source() + f'verify_release_checksum "{archive.as_posix()}" "{sums.as_posix()}" "{ASSET}"\n',
        encoding="utf-8",
        newline="\n",
    )
    return subprocess.run([BASH, script.as_posix()], check=False).returncode


def test_download_path_calls_the_verification_before_extracting():
    text = SETUP.read_text(encoding="utf-8")
    body = text[text.index("download_binary() {"):]
    assert body.index("verify_release_checksum") < body.index("tar -xzf")


@pytest.mark.skipif(BASH is None, reason="bash not available")
def test_checksum_verification_accepts_only_a_single_matching_entry(tmp_path):
    good = hashlib.sha256(b"archive").hexdigest()
    other = hashlib.sha256(b"other").hexdigest()
    assert _verify(tmp_path, f"{good}  {ASSET}\n{other}  unrelated.zip\n") == 0
    assert _verify(tmp_path, f"{good.upper()} *{ASSET}\n") == 0
    assert _verify(tmp_path, f"{good}  {ASSET}\n", archive_bytes=b"tampered") != 0
    assert _verify(tmp_path, f"{other}  unrelated.zip\n") != 0
    assert _verify(tmp_path, "") != 0
    assert _verify(tmp_path, f"{good}  {ASSET}\n{other}  {ASSET}\n") != 0

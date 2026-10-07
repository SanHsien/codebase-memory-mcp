# 開發指南

產品架構與功能見上游文件：[`README.en.md`](../README.en.md)、[`CONTRIBUTING.md`](../CONTRIBUTING.md)。本文只寫本 fork 的維護環境。

## 環境

Windows 11、PowerShell 7、Python 3.12+、Git、GitHub CLI。

## 驗證

```powershell
pwsh -NoProfile -File tools\bootstrap_dev.ps1   # 首次：建 .venv、裝依賴、跑 gate
pwsh -NoProfile -File tools\dev_check.ps1       # 之後
```

gate 依序執行：compileall → ruff → pytest（`tools/tests`）→ 文件連結檢查 → 分岔登記檢查，最後印 `WINDOWS DEV CHECK GREEN`。

gate 只驗維護層。C 程式碼用下方的 MSYS2 步驟本機驗證，也由上游 CI 驗證（`pr.yml`、`dry-run.yml`）。

## 維護工具

| 工具 | 用途 |
|---|---|
| `tools/check_links.py` | 維護文件相對連結 |
| `tools/check_upstream_updates.py` | 上游未審查的 commit／PR／issue |
| `tools/check_divergence.py` | 上游檔案改動與 [`DIVERGENCE.md`](DIVERGENCE.md) 是否一致 |
| `tools/check_dependency_freshness.py` | `requirements-dev.txt` 對 PyPI |

## 本機建置與測試 C（Windows）

需要 MSYS2 CLANG64（`pacman -S --needed mingw-w64-clang-x86_64-{clang,clang-tools-extra,zlib,cppcheck,python} make unzip zip`）。
從 MSYS2 CLANG64 的開始功能表捷徑開啟 shell；從別的 shell 直接叫 `bash.exe` 會丟失 `LOCALAPPDATA`、`USERPROFILE` 等變數，daemon 測試會失敗。

```bash
scripts/build.sh CC=clang CXX=clang++          # 產品二進位
scripts/test.sh --suites cypher CC=clang CXX=clang++   # 單一 suite（增量）
make -f Makefile.cbm lint-format lint-no-suppress      # 格式與 NOLINT 檢查
```

- daemon／IPC 類測試需要只有目前使用者可存取的暫存根：先執行 `scripts/ci/new-protected-temp-root.ps1 -Prefix cbm-ci-tmp- -ProtectDir <repo>\build\c`，再把輸出的路徑設為 `CBM_CI_TEMP_ROOT`、`TMP`、`TEMP`、`TMPDIR`。
- 完整的 `scripts/test.sh` 契約步驟 0r 起需要建立符號連結的權限（開啟 Windows 開發人員模式或以系統管理員執行）。
- `AppData\Local` 帶有額外 ACL 主體（如 AppContainer、沙箱群組）時，daemon 的祖先目錄安全檢查會拒絕，測試在啟動時就失敗（`failed to create isolated test cache and daemon runtime`）或 4 個 `daemon_ipc` 測試失敗。把 `LOCALAPPDATA` 指到使用者目錄下自建的空資料夾即可，例如 `export LOCALAPPDATA='C:\Users\<you>\cbm-la'`。

不要啟用 `core.hooksPath scripts/hooks`（pre-commit 會跑完整 lint、建置與測試）。

## 提交

```powershell
pwsh -NoProfile -File tools\dev_check.ps1 && git add -A && git commit -s -m "type(scope): 說明" && git push origin main
```

`-s` 必要（上游 DCO workflow 檢查 `Signed-off-by`）。改動上游持有的檔案時，同一個 commit 要在 [`DIVERGENCE.md`](DIVERGENCE.md) 登記。

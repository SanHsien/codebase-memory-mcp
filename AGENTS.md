# AGENTS.md

給 AI coding agent 的指引。產品與使用方式見 [`README.md`](README.md)，開發與驗證見 [`docs/DEVELOPMENT.md`](docs/DEVELOPMENT.md)。

## 定位

[`DeusData/codebase-memory-mcp`](https://github.com/DeusData/codebase-memory-mcp)（MIT）的 fork：把程式碼庫索引成本機知識圖譜，透過 MCP 供 AI 工具查詢（純 C、全本機）。`origin` = `SanHsien/codebase-memory-mcp`，`upstream` = DeusData。fork 內容見 [`FORK.md`](FORK.md)，決策見 [`docs/DECISIONS.md`](docs/DECISIONS.md)。

主要環境是 Windows 11 + PowerShell；本機沒有 C 工具鏈，C 由 CI 驗證。

## 硬性邊界

- 不提交索引資料庫、個資、API key、token、`.env`。
- 不推送到 `upstream`；PR、push、release 一律指向 `SanHsien/codebase-memory-mcp`。
- 不移除上游署名、`LICENSE`、`THIRD_PARTY.md` 與 vendored 授權。
- 不動 `src/`、`internal/`、`vendored/`、`tests/`，除非有明確理由；動上游持有檔案要在 [`docs/DIVERGENCE.md`](docs/DIVERGENCE.md) 登記（`tools/check_divergence.py` 會檢查）。
- 不啟用 `core.hooksPath scripts/hooks`。

## 開發原則

- 直接推 `origin/main`，不開功能分支。
- commit 用 Conventional Commit 並加 `-s`。
- 提交前跑 `pwsh -NoProfile -File tools\dev_check.ps1`，通過才提交。
- 文件以繁體中文為主，英文放 `*.en.md`；文件保持精簡，不寫流水帳。
- 修 bug 先補失敗測試再修；修好回註 `REVIEW.md`。
- 同步上游見 [`docs/UPSTREAM.md`](docs/UPSTREAM.md)。
- `requirements-dev.txt` 的紅燈只有兩種出口：宣告行加 `# freshness-hold: <理由>`，或在 `.github/dependency-deferrals.json` 記錄。

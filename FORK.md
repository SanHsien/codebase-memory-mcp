# Fork 說明

fork 自 [`DeusData/codebase-memory-mcp`](https://github.com/DeusData/codebase-memory-mcp)（MIT），保留完整 Git 歷史。產品本體以上游為準，本 fork 只加維護層：

- 繁中 `README.md`（上游英文原文在 `README.en.md`）。
- `AGENTS.md`（`CLAUDE.md`、`GEMINI.md` 為薄補丁）、`NOTICE.md`、`REVIEW.md`、`CHANGELOG.md`。
- Windows 維護 gate（`tools/dev_check.ps1`）、上游追蹤與分岔登記檢查（`tools/`）、對應 CI。
- `docs/`：DEVELOPMENT、DECISIONS、DIVERGENCE、UPSTREAM。

對上游持有檔案的改動登記在 [`docs/DIVERGENCE.md`](docs/DIVERGENCE.md)。

## 分支與 remote

- `origin` = `SanHsien/codebase-memory-mcp`，只有 `main` 一條分支，直接推送。
- `upstream` = `DeusData/codebase-memory-mcp`，只追蹤、不推送。
- 同步方式見 [`docs/UPSTREAM.md`](docs/UPSTREAM.md)。

回貢上游需要維護者當次明確同意。

## 開始開發

```powershell
git clone https://github.com/SanHsien/codebase-memory-mcp.git
cd codebase-memory-mcp
gh repo set-default SanHsien/codebase-memory-mcp
pwsh -NoProfile -File tools\bootstrap_dev.ps1
```

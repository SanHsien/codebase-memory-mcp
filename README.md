[English](README.en.md) | 中文版

# codebase-memory-mcp（SanHsien 維護 fork）

> 本 repo fork 自 [`DeusData/codebase-memory-mcp`](https://github.com/DeusData/codebase-memory-mcp)（MIT）。
> 產品本體以上游為準；本 fork 只加維護骨架與繁體中文入口。差異與同步規則見 [`FORK.md`](FORK.md)、[`docs/DIVERGENCE.md`](docs/DIVERGENCE.md)。
> 上游完整英文說明（安裝、45 種 client 設定、17 個 MCP 工具、圖資料模型）保留在 [`README.en.md`](README.en.md)。

## 這是什麼

把整個程式專案索引成一張**本機知識圖譜**（SQLite），讓 Claude Code、Cursor、Codex 等 AI 寫程式工具直接查圖，不用一個檔案一個檔案翻。

- **省 token**：上游實測 5 次結構化查詢約 3,400 tokens，逐檔搜尋約 412,000 tokens。
- **全部本機執行**：不需 API 金鑰、不需 Docker 或語言 runtime，程式碼不外傳。
- **單一純 C 執行檔**：macOS／Linux／Windows，內含 tree-sitter 語法（上游標示 162 種語言）。
- **17 個 MCP 工具**：搜尋、呼叫追蹤、架構、影響分析、Cypher 查詢、死碼偵測等。
- **內建 3D 圖形介面**：`localhost:9749`。
- 授權 MIT，上游作者 DeusData。

## 快速使用（Windows）

安裝與各 client 設定以上游為準，見 [`README.en.md`](README.en.md) 的 Quick Start：

```powershell
Invoke-WebRequest -Uri https://raw.githubusercontent.com/DeusData/codebase-memory-mcp/main/install.ps1 -OutFile install.ps1
notepad install.ps1   # 先看過腳本再執行
.\install.ps1
```

## 開發環境（本 fork）

主要環境是 **Windows 11 + PowerShell**。本機沒有 C 工具鏈也能跑維護 gate；C 程式的建置與測試交給 CI（上游用 MSYS2 CLANG64）。

```powershell
git clone https://github.com/SanHsien/codebase-memory-mcp.git
cd codebase-memory-mcp
gh repo set-default SanHsien/codebase-memory-mcp
pwsh -NoProfile -File tools\bootstrap_dev.ps1
```

要在本機編譯 C 產品程式，需 MSYS2 CLANG64 工具鏈，步驟見 [`docs/DEVELOPMENT.md`](docs/DEVELOPMENT.md)。

## 文件索引

| 文件 | 內容 |
|---|---|
| [`FORK.md`](FORK.md) | fork 目的、與上游差異、分支與 remote |
| [`AGENTS.md`](AGENTS.md) | AI agent 單一真相源（[`CLAUDE.md`](CLAUDE.md)、[`GEMINI.md`](GEMINI.md) 為薄補丁） |
| [`docs/DEVELOPMENT.md`](docs/DEVELOPMENT.md) | 架構、環境、指令、gate |
| [`docs/DECISIONS.md`](docs/DECISIONS.md) | 決策紀錄 |
| [`docs/DIVERGENCE.md`](docs/DIVERGENCE.md) | 對上游檔案的分岔登記表 |
| [`docs/UPSTREAM.md`](docs/UPSTREAM.md) | 上游同步流程 |
| [`REVIEW.md`](REVIEW.md) | 最新覆核與風險快照 |
| [`CHANGELOG.md`](CHANGELOG.md) | 本 fork 維護歷史 |
| [`NOTICE.md`](NOTICE.md) | 來源與授權 |

## 授權

MIT，見 [`LICENSE`](LICENSE)。保留上游作者署名，見 [`NOTICE.md`](NOTICE.md)。

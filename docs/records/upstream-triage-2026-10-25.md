# 上游 issue／PR 分流（2026-10-25，增量）

範圍：上次基準（PR 2777、issue 2778）之後新增的 PR 4 筆（2779–2781、2783）、issue 4 筆（2785、2787–2789）。上游 `main` 仍是 `10ab613`（2026-10-22），沒有新提交，所以本次不同步程式碼，只推進水位。沒有逐筆重現；本 fork 不在上游留言或開 PR。

## PR

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2783](https://github.com/DeusData/codebase-memory-mcp/pull/2783) | feat(store): hybrid BM25 + vector ranking for semantic_query | OPEN | 動 `store.c`／`mcp.c` 的排序行為，上游尚未合併；合併後同步並跑 `test_mcp` |
| [2781](https://github.com/DeusData/codebase-memory-mcp/pull/2781) | build(deps): Bump actions/upload-artifact from 7.0.1 to 7.0.2 | OPEN | 只改上游 workflow；隨上游合併後同步，不單獨移植 |
| [2780](https://github.com/DeusData/codebase-memory-mcp/pull/2780) | build(deps): Bump github/codeql-action/init from 4.38.1 to 4.39.1 | OPEN | 同上（`codeql.yml`） |
| [2779](https://github.com/DeusData/codebase-memory-mcp/pull/2779) | build(deps): Bump github/codeql-action/upload-sarif from 4.38.1 to 4.39.1 | OPEN | 同上（`scorecard.yml`，本 fork 已停用該 workflow） |

## Issue

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2789](https://github.com/DeusData/codebase-memory-mcp/issues/2789) | TypeScript call resolution: ~3,000 lines of fixes ready … how would you like them split? | OPEN | 貢獻者詢問如何拆 PR，沒有程式碼可採用；上游有 PR 後再看 |
| [2788](https://github.com/DeusData/codebase-memory-mcp/issues/2788) | Cache-root admission compares path strings … | OPEN | 快取目錄搬移加符號連結後 daemon 永久拒絕新 client；Windows 的目錄連接點（junction）情境可能類似。上游尚無修正，有修正 PR 合併後採用 |
| [2787](https://github.com/DeusData/codebase-memory-mcp/issues/2787) | Generated Pi extension (cbmem.ts) breaks Gemini/Antigravity … | OPEN | 產生的 Pi 擴充用 `Type.Union(Type.Literal)`，Gemini 不接受 `const`；維護者目前不用 Pi，等上游修正 |
| [2785](https://github.com/DeusData/codebase-memory-mcp/issues/2785) | Windows: index worker crashes (0xC0000005) in parallel.extract … | OPEN | Windows 索引 worker 存取違規，尚無最小重現；上游有診斷或修正後優先同步 |

觸發條件：上游 `main` 前進、或上述任一 PR 合併，就依 [`UPSTREAM.md`](UPSTREAM.md) 重新同步。

# 上游同步與分流（2026-10-26）

範圍：上游 `main` 由 `10ab613` 前進到 `8530c8c`（103 個 commit，75 檔），加上基準之後新增的 PR 10 筆（2794、2798、2800–2803、2806–2809）與 issue 6 筆（2791、2793、2796、2797、2799、2805）。本 fork 沒有與上游共同祖先，所以用 `10ab613` 當 base 做三方合併；上游持有檔案的分岔仍以 [`DIVERGENCE.md`](../DIVERGENCE.md) 為準。本 fork 不在上游留言或開 PR。

## 合併前掃描

對 `10ab613..8530c8c` 的新增行找藏起來的內容：行內長串空白後的內容、超過 400 字元的單行、`eval`／`new Function(`／`globalThis`、長 base64／hex、零寬與雙向控制字元。命中只有三處超長的 README 文件行與 CLI `--help` 的對齊空白，皆為可見的正常內容。新增的 10 筆 PR 另以 `gh pr diff` 對目前 head 做同樣掃描，無隱藏內容（超長行只出現在文件與一個測試案例字串）。

## 採用的上游變更（整批取用）

| 範圍 | 上游項目 | 備註 |
|---|---|---|
| Cypher | WHERE 屬性對屬性比較（2744）、無分組聚合在空輸入回一列（2764） | `cypher.c` 為三方自動合併，fork 的交叉連接上限與配置檢查沿用 DIVERGENCE 登記 |
| 路由屬性 | 非 UTF-8 的 route properties 與合法 JSON（2738） | 見下方衝突 |
| TypeScript | 推斷接收者呼叫（1999）、`ts_lsp` 走訪堆疊成長（2091）、簽章走訪維持線性 | `ts_lsp.c` 為三方自動合併（含上游 `cbm_walk_stack_reserve`），fork 對該檔的分岔仍登記在 DIVERGENCE；合併後的行為以 `ts_lsp` suite 驗證 |
| 其他語言 | C# 14 extension 成員（2231）、GLSL 階段對應（2761） | |
| Daemon／平台 | cohort 鎖暫存檔清理（2289）、Windows aligned delete（2281）、CLI 偵測 PATH（2381）、session 監看註冊（2737）、已索引專案探索（2508） | `auto_watch` 語意改為全域，`README.en.md` 與 `docs/CONFIGURATION.md` 已同步 |
| 管線 | manifest 走訪略過不可讀子目錄（2085）、移動目錄的文件連結（2602） | |
| CLI／文件 | Codex PreToolUse hook（2719）、`--help` 列 daemon（2443）、install.ps1 區分版面不符（2766）、musl 與連結器（2675） | |
| CI／UI | cppcheck 平行（2755）、CodeQL 警示（2757）、PR 確認不貼隱藏備註（2798）、framework entry flags | `pr-acknowledgement` workflow 本 fork 停用，檔案內容仍同步 |

## 合併衝突與取捨

| 檔案 | 解法 |
|---|---|
| `src/pipeline/pass_route_nodes.c` | HANDLES 邊屬性採上游的 `sveltekit_handles_props`（同一缺陷，fork 先前自己跳脫的版本移除） |
| `tests/test_pipeline.c`（兩處） | 兩邊都保留：fork 的 Windows 裝置名稱檔案測試（`nul`）與上游的不可讀子目錄測試，各自在自己的 `#ifdef` 內 |
| `graph-ui/package.json`、`package-lock.json` | 不採上游：fork 的 `dompurify` 3.4.17 與已升級的 vitest 較新 |
| `README.md`（繁中） | 沒有 watcher 描述，無需併入；上游英文 README 已整份貼進 `README.en.md` |

## PR

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2809](https://github.com/DeusData/codebase-memory-mcp/pull/2809) | fix: isolate test paths and guard empty extraction data | OPEN | 測試隔離與空資料防護，尚未合併；合併後同步 |
| [2808](https://github.com/DeusData/codebase-memory-mcp/pull/2808) | feat(cli): add ZCode as 46th client surface | OPEN | 新增客戶端，本 fork 不用；合併後隨同步進來 |
| [2807](https://github.com/DeusData/codebase-memory-mcp/pull/2807) | fix(cli): emit string enums as Type.String({ enum }) in Pi adapter | OPEN | 對應 issue 2787（Gemini 拒收 `const`）；維護者不用 Pi，合併後同步 |
| [2806](https://github.com/DeusData/codebase-memory-mcp/pull/2806) | Feature/neo4j all tools | CLOSED | 6449 行新增、含 release workflow 與 Docker；上游未收，不採 |
| [2803](https://github.com/DeusData/codebase-memory-mcp/pull/2803) | fix(daemon): re-issue the interrupted request against a replacement daemon | OPEN | 對應 issue 2796（Windows 殺掉一個 endpoint 會帶走共用 daemon）；動 daemon frontend，Windows 為主線，**合併後優先同步並在 Windows 實測** |
| [2802](https://github.com/DeusData/codebase-memory-mcp/pull/2802) | fix(pipeline): never guess a same-named sibling for explicit ./ and ../ imports | OPEN | 對應 issue 2797；修 IMPORTS 邊誤連，合併後同步 |
| [2801](https://github.com/DeusData/codebase-memory-mcp/pull/2801) | feat(cli): give Copilot searches graph context through a PostToolUse hook | OPEN | Copilot 專用，本 fork 不用 |
| [2800](https://github.com/DeusData/codebase-memory-mcp/pull/2800) | fix: guard empty ObjectScript scanner state | OPEN | 對應 issue 2799；只影響 UBSan 下的零長度 `memcpy`，合併後同步 |
| [2798](https://github.com/DeusData/codebase-memory-mcp/pull/2798) | fix(ci): post the PR acknowledgement without its hidden maintainer note | MERGED | 已隨本次同步進來 |
| [2794](https://github.com/DeusData/codebase-memory-mcp/pull/2794) | fix(discover): never index secret-bearing files by default | OPEN | 對應 issue 2793；安全面有價值，但動 14 檔含 discover／index_policy，尚未合併。**合併後優先同步**，不提前移植 |

## Issue

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2805](https://github.com/DeusData/codebase-memory-mcp/issues/2805) | Implement http for mcp natively | OPEN | 功能請求，不採 |
| [2799](https://github.com/DeusData/codebase-memory-mcp/issues/2799) | ObjectScript scanners pass a null empty-state buffer to memcpy | OPEN | 本 fork 同一份 vendored scanner 有相同寫法；等 PR 2800 |
| [2797](https://github.com/DeusData/codebase-memory-mcp/issues/2797) | Nix: `import ../path` resolves to a same-named file | OPEN | 等 PR 2802 |
| [2796](https://github.com/DeusData/codebase-memory-mcp/issues/2796) | Windows: killing one endpoint's process takes down the shared daemon | OPEN | 與本 fork 主環境直接相關；等 PR 2803 |
| [2793](https://github.com/DeusData/codebase-memory-mcp/issues/2793) | Secret-bearing files are indexed and returned to agents | OPEN | 等 PR 2794 |
| [2791](https://github.com/DeusData/codebase-memory-mcp/issues/2791) | CBM Lens renders a .tsx file as its .css sibling | OPEN | 前端 qn 碰撞，尚無修正；有修正 PR 合併後採用 |

觸發條件：上游 `main` 前進、或 PR 2794／2802／2803 合併，就依 [`UPSTREAM.md`](../UPSTREAM.md) 重新同步。

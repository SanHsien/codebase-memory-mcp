# 上游同步與分流（2026-10-28）

範圍：上游 `main` 由 `8530c8c` 前進到 `83109dd`（42 個 commit，其中 10 個是合併 PR 的 merge commit；淨差異 35 檔，+1230／−183），加上基準之後新增的 PR 1 筆（2817）與 issue 2 筆（2814、2816）。本 fork 沒有與上游共同祖先，以 `8530c8c` 當 base 做三方合併（`git apply -3`）；上游 `README.md` 的改動套到 `README.en.md`。本 fork 不在上游留言或開 PR。

## 合併前掃描

對 `8530c8c..83109dd` 的新增行找藏起來的內容：行內長串空白後的內容、超過 400 字元的單行、`eval`／`new Function(`／`globalThis`、長 base64／hex、零寬與雙向控制字元。命中只有 C 程式的參數對齊空白（10 行）與 `vendored/grammars/MANIFEST.md` 一行 1413 字元的授權說明，皆為可見的正常內容。PR 2817 以 `gh pr diff` 對目前 head `8fd93a2e` 做同樣掃描：5 行對齊空白，其餘無命中。

## 採用的上游變更（整批取用）

| 上游 PR | 內容 | 本 fork 對應 |
|---|---|---|
| 1730 | Elixir Phoenix channel 抽取（兩個分支原本都到不了） | `extract_channels.c` 三方自動合併；fork 的「只對純字串包裝節點取頻道名」與接收者 256 位元組上限保留（DIVERGENCE 已登記） |
| 2422 | Python 遞迴 lambda 每條呼叫鏈只走一次本體 | **與 fork 的同一個修正衝突**，採上游版本（見下） |
| 2622 | SQLite 寫入器頁面偏移改 64 位元 | **與 fork 的同一個修正衝突**，採上游版本（見下） |
| 2372 | MCP 請求計時與傳輸失敗記錄 | `mcp.c`、`log.c` 自動合併；fork 對 `mcp.c` 的配置檢查保留 |
| 2555 | JS 範本字串開頭替換的 URL 解析（含 21c224f 後被 a76600d 撤回的常數解析，淨效果只剩測試） | 自動合併 |
| 2724 | JVM 套件名稱像建置輸出目錄（`build`、`out`）時不再被略過 | 自動合併；`test_doc_mentions_jvm.c` 自動合併，fork 對同檔的斷言改動保留 |
| 2777 | `daemon.autoindex.skipped` 記錄實際的 `auto_index_limit` | 自動合併 |
| 2800 | ObjectScript scanner 空狀態不再把 NULL 交給 `memcpy`（上游 issue 2799） | vendored scanner 與 `vendored-checksums.txt` 一起取用 |
| 2801 | Copilot PostToolUse 搜尋 hook | 本 fork 不用 Copilot，但與 CLI 安裝程式同檔，整批取用避免分岔 |
| 2802 | 明確的 `./`、`../` import 不再猜同名兄弟檔（上游 issue 2797） | 自動合併 |

## 合併衝突與取捨

| 檔案 | 解法 |
|---|---|
| `internal/cbm/lsp/py_lsp.c` | 三處衝突全取上游：上游以 `CBMLambdaEntry.expanding` 旗標擋重入，fork 原本是走訪期間把 lambda 名稱暫時設為 NULL。兩者都是「每條呼叫鏈只走一次本體」；fork 的回歸測試見下方驗證。DIVERGENCE 的 `py_lsp.c` 列移除這一項 |
| `internal/cbm/sqlite_writer.c` | 七處 seek 衝突全取上游的 `cbm_writer_seek`／`cbm_writer_page_offset`，fork 的 `page_file_offset`、`writer_seek`、`writer_tell` 因此沒有呼叫者而刪除。fork 仍保留：overflow 頁跳過 pending-byte 頁、發佈前檢查 `ferror`、根頁為 0 即失敗、分隔 cell 的 `malloc` 檢查（上游沒有，DIVERGENCE 列已更新） |

## PR

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2817](https://github.com/DeusData/codebase-memory-mcp/pull/2817) | feat(cli): note when a Claude Code session keeps searching without the graph | OPEN（head `8fd93a2e`） | Claude Code hook 的提示行為，動 `cli.c`、`hook_augment.c`、`tests/test_cli.c`、`scripts/smoke-test.sh`、`README.md`（+407／−17）。尚未合併，不提前移植；合併後隨同步進來。觸發：上游合併或關閉 2817 |

## Issue

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2814](https://github.com/DeusData/codebase-memory-mcp/issues/2814) | Triage main-branch validation failures and glibc 2.44 startup crash | OPEN | 維護者決策用的清單，修正放在 PR 2809（未合併）。最嚴重的一項是 glibc 2.44 上 vendored mimalloc 在建構子之前收到 `free(NULL)` 而崩潰，只發生在 Linux glibc；本 fork 的主環境是 Windows（`mem_override_win.c`）。不提前移植；觸發：上游合併 2809 或 mimalloc 升版 |
| [2816](https://github.com/DeusData/codebase-memory-mcp/issues/2816) | trace_path: expose direct self-recursion instead of an unqualified zero-callee result | OPEN | 回報 `trace_path` 對直接遞迴回 `callees_total: 0` 而沒說明排除了自呼叫，v0.11.0 起就如此，屬上游的圖契約設計，尚無修正。不採；觸發：上游出現對應修正 |

## 驗證

- Windows 11 + MSYS2 CLANG64（ASan＋UBSan）跑合併動到的 12 個 suite（extraction、py_lsp、sqlite_writer、cli、daemon_application、discover、doc_mentions_jvm、edge_imports、grammar_regression、log、mcp、pipeline）：2126 通過、5 失敗、29 略過。
- 5 個失敗在合併前的 `7b0d6463` 同樣失敗（mcp、cli 兩個 suite：697 通過、5 失敗），只差行號，沒有合併造成的新失敗：`cli_install_*` 4 個是本機暫存目錄繼承「Authenticated Users 可修改」被安裝程式拒絕（`acl-grants-cross-account-mutation to S-1-5-11`，見 [`DEVELOPMENT.md`](../DEVELOPMENT.md)）；`detect_changes_scope_tests_answers_test_impact` 在這台機器回 `run_all` 而不是 `nothing`。
- fork 的遞迴 lambda 回歸測試（`pylsp_batchE_recursive_lambda_terminates` 等 4 個）在上游的 `expanding` 防護下通過。
- `tools/dev_check.ps1` 綠。

觸發條件：上游 `main` 前進、或 PR 2809／2817 合併，就依 [`UPSTREAM.md`](../UPSTREAM.md) 重新同步。

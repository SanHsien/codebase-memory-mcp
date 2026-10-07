# REVIEW

最新覆核：2026-10-07。範圍：產品 C 程式碼（`src/`、`internal/cbm/` 非 vendored）、安裝腳本、workflow、測試腳本。

## 方法與環境

- Windows 11 + MSYS2 CLANG64（clang 22.1），ASan＋UBSan 測試 runner。
- 12 個區塊各自獨立讀碼審查（foundation／store、mcp／cypher、pipeline／discover／watcher、cli／daemon／ui、internal/cbm 抽取器、腳本與 workflow、config 編輯器、store 其餘與 supervisor、extract／LSP 本體；第二輪補 `store.c` 全檔、`extract_*.c`、`pass_route_nodes.c` 後段）。
- 每個發現我都對照原始碼驗證，並盡量用真實二進位或真實 git 實測。能重現的先寫會失敗的測試、確認失敗，再修，修後重跑；實測不成立的剔除。

## 測試結果

| | 通過 | 失敗 | 略過 |
|---|---:|---:|---:|
| 修正前（僅補齊環境） | 7726 | 78 | 68 |
| 現在（含同步上游 90 個提交後） | 8197 | 0 | 79 |

另外：`scripts/test.sh` 的全部契約步驟通過；產品二進位回歸（watchdog、worker、scope、字串白名單等）通過。

## 已修正（皆有回歸測試，除非註明）

**資料損毀／資料遺失**

| 問題 | 位置 |
|---|---|
| Hermes（PyYAML 預設）縮排式序列會被插到原項目之前而損毀設定檔 | `src/cli/config_yaml_edit.c` |
| 磁碟機拔除或網路磁碟斷線超過寬限期後，索引資料庫被刪除 | `src/watcher/watcher.c` |
| ADR 空區段取代時，新內容黏到下一個標題上（標題被改名） | `src/store/store.c` |
| 磁碟滿的短寫可能被當成功發佈；頁面偏移在 Windows 超過 2 GiB 寫錯位置（無自動測試） | `internal/cbm/sqlite_writer.c` |
| ADR 段落累加在 8 KiB 緩衝區滿後仍每行寫入換行，越界寫入堆疊（ASan 實測） | `src/store/store.c` |
| 度數批次查詢超過約 2045 個 id 時型別參數綁錯位置，所有節點度數變 0 | `src/store/store.c` |
| 平行路徑（50 檔以上）每條 DATA_FLOWS 的 `route`、`caller_args` 全部遺失（切片少 1 位元組，JSON 驗證失敗後退回最小屬性） | `src/pipeline/pass_route_nodes.c` |
| SvelteKit：layout 與 page 的 `load` 共用一個 Route；monorepo 路由變成 `/app/src/routes/x`；handler 名稱未跳脫 | `src/pipeline/pass_route_nodes.c` |
| `<script>` 後接大量標記時，Svelte／Vue 元件的定義全部遺失（固定 1024 格堆疊丟棄最前面的節點） | `internal/cbm/extract_imports.c` |
| ObjectScript 長 Trigger 本體與 SqlMap globals 產生不合法 JSON | `internal/cbm/extract_defs.c` |
| 覆蓋率影子圖：失敗清空後又出現時，`missed` 圖仍是空的（指紋未更新） | `src/store/store.c` |
| 批次寫入在呼叫端交易中會提交呼叫端的交易；COMMIT 失敗仍回成功 | `src/store/store.c` |
| sqlite_writer 根頁為 0（配置失敗）時仍發佈指向第 0 頁的資料庫（無自動測試） | `internal/cbm/sqlite_writer.c` |
| TOML 跨行陣列裡的 `  [1]` 被當成表頭：移除留下殘缺 TOML、合法檔案被拒 | `src/cli/config_toml_edit.c` |

**當機、卡死、資源耗盡**

| 問題 | 位置 |
|---|---|
| 30 萬個 `AND` 的查詢讓伺服器 stack overflow（ASan 實測崩潰）；UNION 無上限 | `src/cypher/cypher.c` |
| `MATCH (a) MATCH (b)` 交叉連接可耗盡記憶體（上限 25 萬列） | `src/cypher/cypher.c` |
| TS／JS 深度巢狀檔案：ASan 單元測試 600 層即崩潰；真實二進位 8000 層 25 秒 → 5 秒，30000 層超過 5 分鐘 → 105 秒 | `internal/cbm/lsp/ts_lsp.c` |
| 畸形 `Cargo.toml` 的 `members` 使索引無限迴圈 | `internal/cbm/lsp/rust_cargo.c` |
| C 前處理器巨集展開炸彈：1 KB 檔案 2^18 個 token 要 9.6 秒、2^22 個超過 120 秒；展開前估算大小，超過 200 萬 token 就略過展開（防護放在非 vendored 的 `preprocessor.cpp`） | `internal/cbm/preprocessor.cpp` |
| dbt 深巢狀 Jinja 讓 SQL 抽取 stack overflow（ASan 實測）；改為迭代 | `internal/cbm/extract_dbt.c` |
| 內嵌 script 區塊不套用走訪節點上限（無自動測試） | `internal/cbm/extract_imports.c` |
| 11 處配置後未檢查就使用（記憶體不足時 NULL 解參考，無自動測試） | `ac.c`、`sqlite_writer.c`、`cypher.c`、`mcp.c`、`pass_githistory.c`、`pipeline.c`、`registry.c`、`store.c` |
| OPTIONAL MATCH 起點無節點時 heap overflow（上游同期也修了，已採用上游版本） | `src/cypher/cypher.c` |

**Windows 行為錯誤**

| 問題 | 位置 |
|---|---|
| 設定檔原子取代遇到掃描程式暫時鎖檔（錯誤 1175）就放棄，安裝／解除安裝隨機失敗；修正前每輪有 1–5 個隨機失敗，修正後連跑 8 輪全過 | `src/foundation/compat_fs.c` 及四個設定檔編輯器 |
| 中文等非 ASCII 專案路徑下 watcher 靜默失效（窄字元 `stat`） | `src/watcher/watcher.c` |
| `cbm_readdir` 遇到無法轉換的檔名就結束列舉，其後檔案從索引消失 | `src/foundation/compat_fs.c` |
| `cbm_setenv` 在 ANSI 碼頁無法表示的字元上失敗 | `src/foundation/compat.h` |
| `.gitignore` 比對區分大小寫，與 Windows 上 `core.ignorecase=true` 的 git 不一致（已用真實 git 驗證） | `src/discover/gitignore.c` |
| YAML 編輯器在 Windows 以目錄當鎖，崩潰後永久殘留、之後每次編輯都失敗；超過 300 秒的鎖視為失效 | `src/cli/config_yaml_edit.c` |
| Codex hooks 在安裝路徑含空白時永遠裝不起來（上游同期也修了，已採用） | `src/cli/config_toml_edit.c` |

**搜尋、HTTP 介面、安裝腳本**

| 問題 | 位置 |
|---|---|
| 合法 regex（`handle\w+`、`colou?r`）的搜尋漏結果（hint 計算錯誤） | `src/store/store.c` |
| `/api/index` 路徑被截斷可繞過工作區邊界；回應與日誌 JSON 未跳脫 | `src/ui/http_server.c` |
| `setup-windows.ps1` 下載後不驗證雜湊（實測：真檔通過，竄改／缺項／空檔皆拒絕） | `scripts/setup-windows.ps1` |
| 範圍計數把 scope 的 `_`、`%` 當 LIKE 萬用字元（`src/my_pkg` 也計入 `src/myXpkg`）；範圍計數讀取失敗回 0 | `src/store/store.c` |
| QN 後綴查詢 `my_func` 也命中 `a.myXfunc`、`a.MY_FUNC`（`get_code_snippet` 可能回錯節點） | `src/store/store.c` |
| 一筆壞 JSON 的 properties 讓整個架構查詢失敗 | `src/store/store.c` |
| `setup.sh` 下載後不驗證雜湊（`tools/tests/test_setup_sh_checksum.py`） | `scripts/setup.sh` |
| `CBM_MAX_FILE_BYTES` 超過 2 GiB（Windows 的 `long`）時反而退回 512 MiB | `src/foundation/limits.c` |
| `--port=80abc` 被當成 80；`SO_EXCLUSIVEADDRUSE` 失敗仍 bind（無測試） | `src/main.c`、`src/ui/httpd.c` |
| graph_buffer 讀取不檢查結束碼、負 id（防禦性，測試在修正前也通過） | `src/graph_buffer/graph_buffer.c` |
| Perl 早退漏記 `mark_done` | `internal/cbm/cbm.c` |

**測試與工具鏈（讓 Windows 開發者跑得起來）**：`README.md` 改為繁中後上游契約讀取失敗、預設碼頁讀 UTF-8 失敗、MSYS2 python 被當成 Unix、非管理員無法建立受保護暫存根、無符號連結權限時整個契約無法執行、scoop 契約陷阱。

## 審查說法實測後不成立（已剔除）

- JSON 大型陣列 O(N²)：1 萬到 16 萬元素索引時間都約 5 秒。
- k8s 邊屬性 JSON 未跳脫：Helm 範本不會被當成 manifest，實測資料庫無非法 JSON。
- Windows PE 安全旗標：`objdump` 實測 `DYNAMIC_BASE`、`HIGH_ENTROPY_VA`、`NX_COMPAT` 皆已開。
- 「watcher 會因窄字元 `stat` 刪資料庫」：實測回 `EILSEQ` 而非 `ENOENT`，只是失效（已修）；真正會刪的是磁碟機拔除（已修）。
- YAML column-0 清單被插壞：修正前測試即通過，既有程式已處理。
- OneDrive 雲端佔位檔被當成符號連結：本機 OneDrive 無此類檔案，無法重現。
- workflow 的 `${{ inputs.* }}`：只有寫入權限者能手動觸發，輸入非外部可控，且上游契約逐檔檢查標記，不改。
- R 匯入收集的遞迴會 stack overflow：3 萬層巢狀呼叫實測不崩潰。
- JS 深括號 callee 的尾遞迴會 stack overflow：3 萬層實測不崩潰。

## 評估後不改

- `file_pattern`（glob 轉 LIKE）與 `url_path` 搜尋未跳脫 `_`：只會多列出結果，改動需改寫上游測試的預期，不值得。
- DATA_FLOWS 每條路由最多 64 個呼叫端、32 個 infra handler：有意的扇出上限。
- 架構查詢 Louvain 分群 O(E·n)、`bfs_multi` 種子寫入未檢查、gRPC 路由 QN 超長截斷：效能或極端輸入，未見實際影響。
- 架構的路由屬性用字串搜尋解析（非字串值時取錯欄位）：路由屬性由本程式產生，皆為字串。

## 未結

- **`daemon_ipc` 測試**在 `AppData\Local` 帶有額外 ACL 主體（AppContainer、`CodexSandboxUsers`）的機器上失敗：daemon 的祖先目錄安全檢查拒絕。屬環境問題，未動安全邏輯；解法寫在 [`docs/DEVELOPMENT.md`](docs/DEVELOPMENT.md)。
- 抽取器沒有逐行讀完：`extract_defs.c`、`extract_calls.c`、`extract_usages.c`、`extract_unified.c` 大部分只做模式搜尋；各語言 LSP 檔案未審。這些不能視為已排除。
- 上游持續前進：本次同步到 `7d4a12c`（2026-10-02），之後的提交、PR、issue 由 `upstream-check` 持續提示。

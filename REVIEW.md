# REVIEW

最新覆核：2026-10-07。範圍：產品 C 程式碼（`src/`、`internal/cbm/` 非 vendored）、安裝腳本、workflow、測試腳本。

## 方法與環境

- Windows 11 + MSYS2 CLANG64（clang 22.1），ASan＋UBSan 測試 runner。
- 12 個區塊各自獨立讀碼審查（foundation／store、mcp／cypher、pipeline／discover／watcher、cli／daemon／ui、internal/cbm 抽取器、腳本與 workflow、config 編輯器、store 其餘與 supervisor、extract／LSP 本體；第二輪補 `store.c` 全檔、`extract_*.c`、`pass_route_nodes.c` 後段；第三輪逐行讀完抽取器 5 個大檔與全部 18 個 LSP 檔）。
- 第三輪的 94 項審查說法逐項用產品執行檔實測（索引一個只含觸發輸入的 repo，看是否卡死、崩潰或圖錯誤，並以對照組排除案例寫法問題）：58 項重現、約 30 項不成立或屬設計，其餘併入相關修正。
- 每個發現我都對照原始碼驗證，並盡量用真實二進位或真實 git 實測。能重現的先寫會失敗的測試、確認失敗，再修，修後重跑；實測不成立的剔除。

## 測試結果

| | 通過 | 失敗 | 略過 |
|---|---:|---:|---:|
| 修正前（僅補齊環境） | 7726 | 78 | 68 |
| 同步上游到 `e71f23e` 後 | 8739 | 0 | 88 |
| 第三輪修正後 | 8824 | 1 | 88 |

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
| `setup-windows.ps1`、`setup.sh` 下載後不驗證雜湊（上游 2026-10 改由 `install.ps1`／`install.sh` 強制驗證，已採用上游版本） | `scripts/` |
| 範圍計數把 scope 的 `_`、`%` 當 LIKE 萬用字元（`src/my_pkg` 也計入 `src/myXpkg`）；範圍計數讀取失敗回 0 | `src/store/store.c` |
| QN 後綴查詢 `my_func` 也命中 `a.myXfunc`、`a.MY_FUNC`（`get_code_snippet` 可能回錯節點） | `src/store/store.c` |
| 一筆壞 JSON 的 properties 讓整個架構查詢失敗 | `src/store/store.c` |
| `CBM_MAX_FILE_BYTES` 超過 2 GiB（Windows 的 `long`）時反而退回 512 MiB | `src/foundation/limits.c` |
| `--port=80abc` 被當成 80；`SO_EXCLUSIVEADDRUSE` 失敗仍 bind（無測試） | `src/main.c`、`src/ui/httpd.c` |
| graph_buffer 讀取不檢查結束碼、負 id（防禦性，測試在修正前也通過） | `src/graph_buffer/graph_buffer.c` |
| Perl 早退漏記 `mark_done` | `internal/cbm/cbm.c` |

**測試與工具鏈（讓 Windows 開發者跑得起來）**：`README.md` 改為繁中後上游契約讀取失敗、預設碼頁讀 UTF-8 失敗、MSYS2 python 被當成 Unix、非管理員無法建立受保護暫存根、無符號連結權限時整個契約無法執行、scoop 契約陷阱。

## 第三輪：抽取器與 LSP

每項都先寫測試、確認修正前失敗（斷言失敗、ASan 報告、崩潰或逾時），修正後通過。深巢狀輸入在真實索引中會讓 worker 被判定卡死並剔除該檔案（預設判定 15 分鐘，最多重試數次）。

| 範圍 | 修正 |
|---|---|
| 定義抽取 | C++ 3 萬層模板鏈 stack overflow；Elixir `when` 守衛、`defimpl`／`defprotocol` 的函式遺失，不同模組同名函式被合併；一行宣告多個名稱只留第一個（Go 欄位與變數、Java 欄位、C 全域）；TS 裝飾器之間有註解時路由消失；2 萬個加裝飾器的方法 TS 28 秒、Python 49 秒 |
| 呼叫、頻道、用法 | Clojure 5000 層巢狀卡死；3 萬段 JS 方法鏈卡死；Go 反引號與 Python 前綴字串的路由遺失；`"user:" + id` 被記成頻道 `user:`；Go 大量遮蔽綁定 39 秒 |
| 共用走訪與作用域 | 每個節點掃整個作用域堆疊（3 萬段鏈 59 秒→4 秒）；同一作用域大量名稱平方時間（6 萬行 39 秒→9 秒）；深層 JSON 180 秒→0.5 秒；YAML 超過 256 個巢狀鍵遺失資料；C++／Java／TS 深泛型與深層 YAML 的用法分類平方以上（Java 31 秒→0.5 秒） |
| C/C++ LSP | 深命名空間 stack overflow；循環繼承成員查找卡死；未知作用域 `X::f()` 以 0.90 信心綁到無關自由函式 |
| Java／Go LSP | 巢狀裸區塊與長 `+` 鏈指數時間；深泛型、深巢狀類別、Go 長運算式 stack overflow；Go 巢狀閉包指數時間；遮蔽欄位的區域變數被綁到欄位型別 |
| Python LSP | 兩行遞迴 lambda 70 秒、21 GB；walrus、括號、深註解型別 stack overflow；`elif` 之後的 `else` 未解析；`list[A] \| None` 回傳型別失效 |
| TS LSP | 深泛型型別文字 stack overflow；未標註回傳型別的函式內呼叫遺失；`??`、`\|\|` 被當成 number；`reduce` 累加器型別錯；跨檔 union 回傳型別被切成 tuple；回呼參數越界讀取（ASan）；大型 union 攤平遺失成員 |
| Kotlin／Rust LSP | 深 lambda、if、類別、else-if 鏈 stack overflow 或卡死；`!is` 也做窄化；else 分支未解析；深泛型回傳型別 stack overflow；`impl<'a>` 的呼叫歸到檔案；rustdoc 堆疊緩衝區越界讀取等 3 個 ASan 錯誤 |
| PHP／C# LSP | bindTo 巢狀指數時間；`&&` 鏈、括號、大括號、callee 深巢狀 stack overflow；6000 個方法的類別 77 秒；第 256 個頂層節點後的類別欄位遺失；C# 深巢狀 stack overflow；`_svc.Config.Get()` 綁到無關型別；型別索引每檔洩漏到長期 arena |
| 其他 | 呼叫歸屬的「呼叫×定義」迴圈改為二分搜尋 |

多處深度上限沒有生效的原因相同：區域的括號計數變數也叫 `depth`，遮蔽了新的深度參數（C、Rust 各一處，已改名）。

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
- 第三輪另有：SQL 巢狀子查詢、Go 括號鏈、PHP phpdoc 泛型與 trait 爆炸、Rust `+` 鏈與巨集、Kotlin 循環父型別、TS 循環介面、C# 括號鏈、C# 一行多欄位、Python metaclass 基底、Go map 參數型別、Go 嵌入深度與 type switch、Python lambda 依名稱登錄、Perl `shift` 綁定（同名邊來自名稱比對）、C 結構與函式同名，實測都不成立。

## 評估後不改

- `file_pattern`（glob 轉 LIKE）與 `url_path` 搜尋未跳脫 `_`：只會多列出結果，改動需改寫上游測試的預期，不值得。
- DATA_FLOWS 每條路由最多 64 個呼叫端、32 個 infra handler：有意的扇出上限。
- 架構查詢 Louvain 分群 O(E·n)、`bfs_multi` 種子寫入未檢查、gRPC 路由 QN 超長截斷：效能或極端輸入，未見實際影響。
- 架構的路由屬性用字串搜尋解析（非字串值時取錯欄位）：路由屬性由本程式產生，皆為字串。
- JS `"/users/" + id + "/posts"` 記成 `/posts`：上游測試（issue #1249）明確要求「取最後一段字面路徑」，屬刻意設計。

## 同步後改用上游版本

2026-10-16 同步時，上游自己修好以下問題，本 fork 的修正改用上游版本：Cypher AND/OR/XOR 長鏈與 UNION 長鏈（上游改成鏈結串列與迴圈，不再需要長度上限）、同一作用域大量名稱的平方時間（上游 #1527 的名稱索引）、呼叫歸屬的「呼叫×定義」迴圈。

## 未結

- **`daemon_ipc` 測試**在 `AppData\Local` 帶有額外 ACL 主體（AppContainer、`CodexSandboxUsers`）的機器上失敗：daemon 的祖先目錄安全檢查拒絕。屬環境問題，未動安全邏輯；解法寫在 [`docs/DEVELOPMENT.md`](docs/DEVELOPMENT.md)。
- C++ `std::vector<` 8000 層這類輸入仍要約 49 秒，其中 46 秒在 vendored 的 tree-sitter C++ 解析器本身（不在本 fork 修改範圍）。
- `py_lsp_scale` 的比例檢查（2000/100 < 200 倍）在 24 個平行工作下會失敗，單獨重跑通過（75.6 倍）。同負載下各跑 12 份對照：修正後 2000 個函式約 19.4 秒，未修改的版本約 31.6 秒，比例相近，屬上游既有的負載敏感測試（上游註解：已知超線性，#1527）。
- 上游持續前進：最近一次同步到 `ec6427c`（2026-10-16，v0.12.0，707 個新提交），之後的提交、PR、issue 由 `upstream-check` 持續提示；新增 PR／issue 的分流見 [`docs/records/upstream-triage-2026-10-16.md`](docs/records/upstream-triage-2026-10-16.md)。

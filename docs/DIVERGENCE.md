# 分岔登記表

本 fork 改動過的**上游持有檔案**，每個檔案一列。新增的檔案（如 `AGENTS.md`、`tools/*.py`）不算。

[`tools/check_divergence.py`](../tools/check_divergence.py) 會比對「自基準 commit（`tools/upstream_baseline.json`）起改過或刪過的上游檔案」與本表，不一致就非 0 退出（已接進 `dev_check.ps1` 與每週的 `upstream-check`）。

最後一欄寫成可執行的判準，同步上游時照做，不用重新評估。

| 上游檔案 | 上游原狀 | 本 fork 狀態 | 為什麼分岔 | 跟進上游時怎麼處理 |
|---|---|---|---|---|
| `README.md` | 英文 README | 改寫為繁中精簡入口 | 公開入口以繁中為主；原文保留在 `README.en.md` | 把上游新增的產品事實（工具數、語言數、安裝指令）併入本檔，不整份覆蓋 |
| `README.en.md` | 不存在（原文就是 `README.md`） | 上游 `README.md` 原文，第一行換成語言切換列 | 保留英文原文 | 用上游新版全文覆蓋，再把第一行換回語言切換列 |
| `.gitignore` | 上游忽略規則 | 尾端加 `# --- fork ---` 區塊：`!CHANGELOG.md` 與本機 gate 產物 | 上游忽略 `CHANGELOG.md`，但本 fork 的 `CHANGELOG.md` 需入版控；gate 會產生 `.venv/` 等檔 | 上游新增規則併在 fork 區塊之上；上游若自己處理同一項，刪掉重複行 |
| `src/cypher/cypher.c` | 交叉連接只擋 INT_MAX；`expr_binary` 配置未檢查 | 交叉連接中間列數上限 25 萬；配置失敗時釋放左右子樹並回傳 NULL，AND/OR/XOR 鏈結時檢查 | `MATCH (a) MATCH (b)` 可耗盡記憶體（有回歸測試）；記憶體不足時 NULL 解參考。AND/OR/XOR 長鏈與 UNION 長鏈的 stack overflow、OPTIONAL MATCH 的 heap overflow 上游已自己修好（2026-10 同步時採用上游版本） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/foundation/compat.h` | Windows `cbm_setenv` 先呼叫 `_putenv_s`，失敗就整個返回 | `_putenv_s` 失敗（ANSI 碼頁無法表示的字元，EILSEQ）不再中止，仍以寬字元 API 設定 | UTF-8 環境變數在非 UTF-8 碼頁的 Windows 上設定失敗 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/ui/http_server.c` | `/api/index` 以截斷後的路徑建索引、回應與 `/api/browse`、日誌輸出未完整跳脫 | 拒絕超過 job slot 的路徑；補跳脫；補一處 doc 洩漏 | 截斷可繞過工作區邊界；Windows 路徑的回應不是合法 JSON（`/api/index` 回應的跳脫上游已自己修好，2026-10-16 同步時採用上游版本） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/watcher/watcher.c` | 根目錄判為不存在（ABSENT）就算已刪除 | Windows 上磁碟機不存在（拔除的隨身碟、移除的磁碟機代號）或 UNC 路徑時視為「不確定」而非已刪除 | 磁碟機拔除超過寬限期後索引資料庫被刪除。UTF-8 路徑上游已自己修好（#2639，2026-10-19 同步時採用上游版本，fork 的 `watcher_stat` 移除） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/rust_cargo.c` | `members` 陣列遇到未加引號的項目無限迴圈 | 該輪未前進時強制前進一個字元 | 畸形 Cargo.toml 使索引 worker 卡死 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/preprocessor.cpp` | 直接交給 simplecpp 展開，巨集展開大小無上限 | 展開前單趟估算 object-like 巨集的展開 token 數，超過 200 萬就回傳 NULL（呼叫端改用原始碼） | 1 KB 檔案 `#define A1 A0 A0 …` 可展開成 2^N 個 token（2^22 超過 2 分鐘）；simplecpp 為 vendored 且有雜湊保護，故防護放在非 vendored 的此檔（有回歸測試） | 上游若自己加上展開上限，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_extraction.c` | — | 新增巨集展開炸彈測試（須在 20 秒內完成且仍找到真實函式）、正常巨集檔不受影響；Svelte 前置 script、長 Trigger 本體 JSON 測試、批次 A 回歸測試 | 對應 preprocessor.cpp 修正 | 隨 preprocessor.cpp 一起處理 |
| `src/foundation/limits.c` | `CBM_MAX_FILE_BYTES` 超出 `long` 範圍時回到 512 MiB 預設 | 溢位時用 `LONG_MAX` | Windows 的 `long` 為 32 位元，設定 3 GiB 反而變成 512 MiB（有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_index_resilience.c` | — | 新增 `CBM_MAX_FILE_BYTES` 溢位測試 | 對應 limits.c 修正 | 隨 limits.c 一起處理 |
| `internal/cbm/ac.c` | `cbm_ac_build` 配置未檢查 | 任一配置失敗即釋放並回傳 NULL | 記憶體不足時 NULL 解參考（無自動測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/mcp/mcp.c` | 讀取片段時路徑緩衝配置未檢查 | 配置失敗回傳 NULL | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/pipeline/pass_githistory.c` | 共同變更配對的配置未檢查 | 配置失敗略過該配對 | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/pipeline/pipeline.c` | `create_folder_chain` 的 `strdup` 未檢查 | 失敗即返回 | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/pipeline/registry.c` | 後綴查詢的配置未檢查 | 失敗回傳 0 筆 | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/pipeline/pass_route_nodes.c` | `"args":[` 切片少 1 位元組；SvelteKit 的 layout load 與 page load 共用同一 Route；路由根取第一個 `/routes/`；handler、service 名稱未跳脫 | 切片長度改為 8；layout load 用 `LAYOUT_LOAD`；路由根優先取 `src/routes/`；名稱經 `cbm_json_escape` 並加大緩衝區 | 平行路徑（50 檔以上）每條 DATA_FLOWS 都退回成 `{via, edge_type}`，`route` 與 `caller_args` 全部遺失；layout 的 load 被當成 page 的 handler；monorepo 路由變成 `/app/src/routes/x`（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_edge_structural.c` | — | 新增 DATA_FLOWS 屬性、SvelteKit 兩個路由測試 | 對應 pass_route_nodes.c 修正 | 隨 pass_route_nodes.c 一起處理 |
| `internal/cbm/extract_dbt.c` | Jinja 樹的 ref 收集與最右字串搜尋為無上限遞迴 | 改用 `TSNodeStack` 迭代 | 深巢狀 Jinja 使 SQL 抽取 stack overflow（ASan 實測） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_stack_overflow.c` | — | 新增 dbt 深巢狀測試、批次 B／U／V 的效能與深巢狀測試 | 對應 extract_dbt.c 修正 | 隨 extract_dbt.c 一起處理 |
| `internal/cbm/extract_imports.c` | 內嵌 script 搜尋用 1024 格固定堆疊，滿了就丟棄最前面的兄弟節點；內嵌區塊走訪不套用節點預算 | 改用可成長的 `TSNodeStack`；子走訪沿用並回報節點預算 | `<script>` 後面接大量標記時，元件的定義全部遺失（有回歸測試）；巨大 script 區塊繞過走訪上限 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/extract_defs.c` | ObjectScript Trigger 本體寫入 512 位元組緩衝區、tab 未跳脫；SqlMap globals 未跳脫 | 緩衝區留足包裝空間、跳脫 tab 與控制字元；globals 複製時跳脫；C++ 模板鏈改迭代；Elixir `when` 守衛、`defimpl`／`defprotocol`、函式 QN 帶模組；一行多名稱宣告（Go 欄位與變數、Java 欄位、C 全域）全部輸出；裝飾器間的註解不再中斷路由；doc memo 的 LRU 修正；同一行宣告的多個欄位只有第一個帶文件註解 | 長 Trigger 的 docstring 是不合法 JSON（有回歸測試）；深模板鏈 stack overflow、Elixir 定義遺失或合併、第二個以後的名稱遺失、TS 路由遺失、2 萬個裝飾器方法平方時間（皆有回歸測試）；註解複製到每個名稱時，上游的文件連結會把同一段註解的參照重複算（C# 實測 2 條變 6 條，有上游測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_store_nodes.c` | — | 新增 QN 後綴精確比對、覆蓋率影子圖重建、批次寫入交易測試 | 對應 store.c 修正 | 隨 store.c 一起處理 |
| `internal/cbm/extract_calls.c` | 字串字面值只剝雙／單引號；callee 文字為整條接收者鏈；Lisp 類語言每個呼叫都回溯全部祖先 | 剝反引號與 r/f/b/u 前綴、三引號；callee 文字超過 512 位元組只留末段；祖先回溯上限 32 層並用走訪游標 | Go 反引號路由遺失；3 萬段 JS 方法鏈卡死；Clojure 5000 層巢狀卡死（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/extract_channels.c` | 第一個參數的任何節點都取第一段字串當頻道名 | 只對純字串包裝節點取值；接收者文字上限 256 位元組 | `"user:" + id` 被記成頻道 `user:`（有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/extract_usages.c` | 識別字分類對每個節點回溯祖先；遮蔽綁定查找不提前結束；Lisp 定義判斷回溯全部祖先 | 祖先資訊改由走訪維護的堆疊提供；綁定查找依排序提前結束；作用域查找 O(1) | C++／Java／TS 深泛型與深初始化串列的抽取為平方以上；Go 大量遮蔽綁定變慢（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/extract_unified.c` | 每個節點掃描整個作用域堆疊；YAML infra 掃描在每層容器重掃子樹且有固定堆疊上限；巢狀 YAML 重複走訪 | 作用域框架記錄最近的詞法／函式框架；infra 掃描每個 mapping 一次並去重；YAML 走訪用可成長堆疊、每節點有限次；Elixir 函式的作用域 QN 改用與定義端相同的模組限定名 | 深巢狀輸入平方時間；深層 JSON 180 秒；YAML 超過 256 個巢狀鍵遺失資料（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/extract_unified.h` | — | `WalkState` 與作用域框架新增欄位 | 支援上兩列的 O(1) 查找 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/c_lsp.c` | 命名空間巢狀、型別文字無深度上限；成員查找無訪問上限；未知作用域的 `X::f()` 以裸名綁到自由函式 | 深度上限；查找節點預算 256；裸名退路只用於已知命名空間 | 深巢狀 stack overflow、循環繼承卡死、錯誤呼叫邊（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/cs_lsp.c` | 區塊、命名空間、類別巢狀無深度上限；點號接收者以最後一段比對短型別名 | 深度上限 256；短名退路只用於無點號接收者，先解析欄位與區域變數 | 深巢狀 stack overflow；`_svc.Config.Get()` 綁到無關的 `App.Config`（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/go_lsp.c` | 閉包本體被求值與走訪各一次；運算式與型別求值無深度上限 | 已求值的閉包不再走訪；深度上限 | 巢狀閉包指數時間；長運算式 stack overflow（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/java_lsp.c` | 裸區塊重複處理；字串串接判斷重複求值左運算元；泛型與類別巢狀無深度上限；未知型別的區域變數退回同名欄位 | 各修正並加深度上限 128 | 指數時間或 stack overflow；`x.run()` 綁到被遮蔽欄位的型別（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/kotlin_lsp.c` | lambda／if／陳述式循環不計深度；類別巢狀、else-if 鏈、型別解析無上限；`!is` 也做窄化；if 用不存在的 `then` 欄位 | 深度上限；`!is` 不窄化；改用 consequence／alternative | stack overflow 或卡死；`!is Dog` 綁到 `Dog.speak`；else 分支呼叫未解析（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/php_lsp.c` | bindTo 閉包走訪兩次；`&&` 鏈、括號、巢狀大括號、callee 解析無深度上限；前置 phpdoc 查找 O(idx²)；類別欄位收集固定 256 格堆疊 | 跳過已走訪子節點；深度上限；改向前兄弟節點；可成長堆疊 | 指數時間、stack overflow、6000 個方法 77 秒、第 256 個頂層節點後的類別欄位遺失（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/py_lsp.c` | 遞迴 lambda 每個呼叫點重走本體；walrus 綁定、括號、註解型別無深度上限；只走訪第一個 elif；`A \| None` 的管線旗標被重設 | 重入保護；深度上限並以迴圈拆括號；走訪所有 elif／else；修正管線判斷 | 兩行檔案 70 秒；stack overflow；else 分支呼叫未解析；PEP 604 回傳型別失效（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/rust_lsp.c` | 型別文字解析的深度參數被同名區域變數遮蔽 | 更名並讓上限生效 | 深泛型 stack overflow（有回歸測試）。泛型 impl 的 self 型別保留 `<'a>` 上游已自己修好（2026-10-19 同步時採用上游版本，fork 的回歸測試保留） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/rust_rustdoc.c` | `join_path` 未夾住 snprintf 回傳值；型別解析無深度上限 | 夾住位移；深度上限 | ASan 實測堆疊緩衝區越界讀取、use-after-scope、stack overflow（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/type_registry.c` | 方法索引配置在 registry 的長期 arena | 改用索引用的暫存 arena | 每個檔案洩漏到整次索引的 arena（有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/type_rep.c` | union 攤平緩衝區上限 count*2+4 | 依實際成員數配置 | 大型巢狀 union 遺失成員（有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_c_lsp.c` | — | 新增回歸測試 | 對應 c_lsp.c 修正 | 隨 c_lsp.c 一起處理 |
| `tests/test_cs_lsp.c` | — | 新增回歸測試 | 對應 cs_lsp.c 修正 | 隨 cs_lsp.c 一起處理 |
| `tests/test_go_lsp.c` | — | 新增回歸測試 | 對應 go_lsp.c 修正 | 隨 go_lsp.c 一起處理 |
| `tests/test_java_lsp.c` | — | 新增回歸測試、裸區塊作用域復原測試 | 對應 java_lsp.c 修正 | 隨 java_lsp.c 一起處理 |
| `tests/test_kotlin_lsp.c` | — | 新增回歸測試 | 對應 kotlin_lsp.c 修正 | 隨 kotlin_lsp.c 一起處理 |
| `tests/test_php_lsp.c` | — | 新增回歸測試 | 對應 php_lsp.c 修正 | 隨 php_lsp.c 一起處理 |
| `tests/test_py_lsp.c` | — | 新增回歸測試 | 對應 py_lsp.c 修正 | 隨 py_lsp.c 一起處理 |
| `internal/cbm/helpers.h` | — | 宣告 `cbm_elixir_unwrap_guard`、`cbm_elixir_module_name` | 定義端與作用域追蹤共用同一套 Elixir 函式 QN 規則，否則函式內的呼叫邊遺失 | 隨 extract_defs.c 一起處理 |
| `src/pipeline/pipeline_incremental.c` | 控制檔掃描對每個目錄項目取檔案資訊，任一失敗就讓整個語意 manifest 失敗 | 只對目錄與控制檔名取檔案資訊，其餘依目錄列舉的類型略過（POSIX 類型未知時照舊） | Windows 上名為 `nul` 的檔案開到 NUL 裝置，每次索引都 `aborted_previous_preserved`（上游 issue #2646，本機實測重現；有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_pipeline.c` | — | 新增 Windows `nul` 檔案的 manifest 回歸測試 | 對應 pipeline_incremental.c 修正 | 隨 pipeline_incremental.c 一起處理 |
| `internal/cbm/cbm.c` | Perl 巢狀過深的早退未記 `mark_done` | 補 `cbm_index_mark_done` | 正常略過被 supervisor 誤判為 crash 嫌疑。呼叫歸屬的「呼叫×定義」迴圈上游已自己改快（2026-10-16 同步時採用上游版本） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `scripts/ci/new-protected-temp-root.ps1` | 以 `Set-Acl` 設定 ACL | 改用 `DirectoryInfo.SetAccessControl` | `Set-Acl` 需要 SeSecurityPrivilege，非管理員無法執行測試 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_cypher.c` | — | 交叉連接上限斷言 | 對應 cypher.c 修正 | 隨 cypher.c 一起處理 |
| `tests/test_httpd.c` | — | 新增兩個 index 回歸測試；過長路徑測試改用超過 4096 位元組的路徑 | 對應 http_server.c 修正；上游把路徑正規化後存入，`/./` 填充的短路徑不再算過長 | 隨 http_server.c 一起處理 |
| `tests/test_doc_mentions_jvm.c` | 斷言同一行的第二個 Java 欄位沒有定義 | 改為有自己的定義 | 本 fork 讓每個名稱都產生欄位（extract_defs.c） | 隨 extract_defs.c 一起處理 |
| `tests/test_watcher.c` | — | 新增非 ASCII 根目錄測試、磁碟機不存在不修剪測試 | 對應 watcher.c 修正 | 隨 watcher.c 一起處理 |
| `tests/test_rust_lsp.c` | — | 新增畸形 `members` 測試、批次 G1 回歸測試 | 對應 rust_cargo.c 修正 | 隨 rust_cargo.c 一起處理 |
| `tests/test_cli.c` | 讀取根目錄 `README.md` 驗證支援的 agent 清單 | 改讀 `README.en.md` | `README.md` 已改為繁中入口，上游原文在 `README.en.md` | 上游改讀哪個檔都跟著改回讀 `README.en.md` |
| `tests/test_language_count_contract.sh` | 檢查 `README.md` 的語言數 | 改檢查 `README.en.md` | 同上 | 同上 |
| `tests/test_vt_gate_policy_contract.sh` | 檢查 `README.md` 的 VirusTotal 政策文字 | 改檢查 `README.en.md` | 同上 | 同上 |
| `tests/test_smoke_fixture_contract.sh` | 以 `sys.platform != "win32"` 判斷是否為原生 Windows | 同時排除 `cygwin`、`msys` | MSYS2 的 python 回報 `cygwin`，Unix 專用檢查在 Windows 上誤跑 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_venue_parity_contract.sh` | `read_text()` 未指定編碼 | 指定 `encoding="utf-8"` | 預設碼頁（如 cp950）無法解碼 UTF-8 檔案 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_release_gate_chain_contract.sh` | `read_text()` 未指定編碼 | 指定 `encoding="utf-8"` | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_select_lanes.sh` | 讀子程序輸出未指定編碼 | 指定 `encoding="utf-8"` | 非 UTF-8 碼頁（如 cp950）的 Windows 上解碼失敗，契約整步中止 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_vt_release_notes_contract.sh` | `read_text()` 未指定編碼 | 指定 `encoding="utf-8"` | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/store/store.c` | `cbm_extract_like_hints` 把 `\w`、`\d` 當字面字母、把可省略字元當必要片段；ADR 區段取代在空區段時把新內容黏到下一個標題；範圍計數把 scope 中的 `_`、`%` 當 LIKE 萬用字元；scoped 計數讀取失敗回 0；度數批次查詢超過約 2045 個 id 時型別參數綁錯位置；QN 後綴查詢以 LIKE 比對；架構查詢的 `json_extract` 未防護；ADR 段落累加在緩衝區滿後仍寫入換行；覆蓋率影子圖清空時不更新指紋；批次寫入在外層交易中會提交外層交易 | `\` 加英數視為 meta；`? * {` 前一字元不納入；`)` 後接 `? * {` 不產生 hint。ADR 取代在需要時補上文件自己的換行；scope 前綴跳脫 `\`、`%`、`_` 並加 `ESCAPE`；讀取失敗回 `CBM_STORE_ERR`（與非 scoped 版本一致）；社群 `edge_types` 檢查配置；度數分塊查詢；後綴改為跳脫的 LIKE 加精確尾端比對；`json_extract` 先經 `json_valid`；換行計入邊界檢查；所有清空路徑都寫入指紋；批次寫入加入呼叫端交易、檢查 BEGIN／COMMIT | hint 是 AND 過濾，錯誤 hint 讓合法 regex 搜尋漏結果；ADR 標題被改名；scope `src/my_pkg` 也計入 `src/myXpkg`（皆有回歸測試）；讀取失敗被當成空範圍；度數全為 0；`my_func` 查到 `a.myXfunc`；一筆壞 JSON 讓整個架構查詢失敗；堆疊緩衝區越界寫入（ASan 實測）；失敗回來後 missed 圖仍空；呼叫端無法回滾（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_store_search.c` | — | 新增 hint 邊界案例斷言 | 對應 store.c 修正 | 隨 store.c 一起處理 |
| `src/graph_buffer/graph_buffer.c` | `cbm_gbuf_load_from_db` 不檢查 `sqlite3_step` 結束碼、不檢查 id 是否為負 | 讀取未以 SQLITE_DONE 結束就回傳失敗；負 id 不寫入重新對應表 | 讀取中途失敗會得到被截斷的圖並被後續寫回覆蓋資料庫；負 id 造成越界寫入（無法以測試重現，屬防禦性修正） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_graph_buffer.c` | — | 新增負節點 id 測試 | 對應 graph_buffer.c 修正（修正前也通過，僅作冒煙測試） | 隨 graph_buffer.c 一起處理 |
| `src/foundation/compat_fs.c` | Windows `cbm_readdir` 遇到無法轉成 UTF-8 的檔名就結束整個目錄列舉 | 略過該項目並繼續列舉；新增 `cbm_win_replace_file_retry`（暫時性共享衝突時重試的原子取代） | NTFS 允許孤立代理字元檔名，其後檔案從索引消失；掃描程式短暫鎖檔會讓 ReplaceFileW 回 1175，設定檔寫入隨機失敗 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_platform.c` | — | 新增孤立代理字元檔名列舉測試、掃描程式鎖檔時取代重試測試 | 對應 compat_fs.c 修正 | 隨 compat_fs.c 一起處理 |
| `src/main.c` | `daemon --port=` 以 `atoi` 解析 | 改用 `strtol` 並檢查結尾與範圍 | `80abc` 被當成 80；溢位為未定義行為 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/ui/httpd.c` | Windows 設定 `SO_EXCLUSIVEADDRUSE` 失敗時仍繼續 bind | 失敗即關閉 socket 並回傳 NULL | 失敗會退回一般 bind，其他本機使用者可搶佔連接埠 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/foundation/compat_fs.h` | — | 宣告 `cbm_win_replace_file_retry`（Windows） | 供四個設定檔編輯器共用 | 隨 compat_fs.c 一起處理 |
| `src/cli/config_text_edit.c` | Windows 以 `ReplaceFileW`／`MoveFileExW` 取代，失敗即放棄 | 改呼叫 `cbm_win_replace_file_retry` | 掃描程式或索引器短暫握住檔案時（錯誤 1175）安裝／解除安裝隨機失敗 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/cli/config_json_like.c` | 同上 | 同上 | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/cli/config_toml_edit.c` | Windows 以 `ReplaceFileW`／`MoveFileExW` 取代，失敗即放棄；跨行陣列內以 `[` 開頭的行被當成表頭 | 改呼叫 `cbm_win_replace_file_retry`；掃描狀態一併追蹤未閉合的 `[`，陣列內不解析表頭；值為跨行陣列的鍵不就地取代 | 掃描程式或索引器短暫握住檔案時（錯誤 1175）安裝／解除安裝隨機失敗。Codex 路徑含空白的問題上游已自己修好（採用上游版本）；跨行陣列的元素 `  [1]` 截斷所屬表，移除後留下殘缺 TOML、合法檔案被拒（有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/cli/config_yaml_edit.c` | 同上；縮排式序列（`key:` 下的 dash 與 key 同縮排）被當成區段終點；Windows 以目錄當鎖，崩潰後永久殘留 | 同上；遇到該形式回傳錯誤、不修改檔案；超過 300 秒未更新的鎖目錄視為失效並移除 | Hermes（PyYAML 預設輸出）設定檔會被插入到原項目之前而損毀；崩潰後每次 YAML 編輯（安裝與解除安裝）都永久失敗 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_config_yaml_edit.c` | — | 新增兩個「不得損毀」測試、一個過期鎖回收測試（Windows） | 對應 config_yaml_edit.c 修正 | 隨 config_yaml_edit.c 一起處理 |
| `tests/test_config_toml_edit.c` | — | 新增含空白安裝路徑測試、跨行巢狀陣列測試 | 對應 config_toml_edit.c 修正 | 隨 config_toml_edit.c 一起處理 |
| `tests/test_vt_candidate_selection_contract.sh` | 無條件建立符號連結測試案例 | 先探測能否建立符號連結，不能就略過該案例 | Windows 非管理員／未開開發人員模式時 `os.symlink` 回 WinError 1314，整個契約無法執行 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_store_arch.c` | — | 新增 ADR 空區段取代測試、scope 萬用字元字面比對測試、scoped 計數讀取失敗測試、度數分塊、壞 JSON 架構、ADR 越界測試 | 對應 store.c 修正 | 隨 store.c 一起處理 |
| `internal/cbm/sqlite_writer.c` | 頁面偏移用 `long`（Windows 為 32 位元）；overflow 頁配置不跳過 pending-byte 頁；發佈前只看 `fflush`；資料表與中繼資料表根頁為 0（配置失敗）時仍寫出；分隔 cell 的 `malloc` 未檢查 | 偏移與 seek 改 64 位元；overflow 頁跳過 pending-byte 頁；發佈前同時檢查 `ferror`；根頁為 0 即失敗不發佈；檢查 `malloc` | 資料庫超過 2 GiB（Windows）或 1 GiB 時頁面寫錯位置；磁碟滿的短寫可能被當成功發佈（無自動測試：需要大型資料庫）；記憶體不足時可能發佈指向第 0 頁的損毀資料庫（無自動測試：需注入配置失敗） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/ts_lsp.c` | TS／JS 的 `process_node` 遞迴沒有深度上限（Go、Java、Python、C 的 LSP 都有） | 加入 `walk_depth` 守門（`CBM_LSP_MAX_WALK_DEPTH` 的一半，預設 256：TS 每層的堆疊用量約為其他語言的兩倍，512 層在 ASan 下會用完 8 MiB 主執行緒堆疊），超過就略過該子樹；型別文字深度上限；推斷回傳型別後清 memo；`??`／`\|\|`／`&&` 不再一律視為 number；`reduce` 累加器型別；跨檔回傳型別不以 `\|` 切開；回呼參數不越界讀取 | 8000 層巢狀 `if` 的索引 25 秒、30000 層超過 5 分鐘不完成，且有棧溢位風險（ASan 單元測試 600 層即崩潰）；深泛型 stack overflow、未標註回傳型別的函式內呼叫遺失、型別錯誤導致錯誤邊、ASan 實測越界讀取（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/ts_lsp.h` | — | `TSLSPContext` 新增 `walk_depth` | 供 ts_lsp.c 的守門使用 | 隨 ts_lsp.c 一起處理 |
| `tests/test_ts_lsp.c` | — | 新增深度守門測試、批次 F 回歸測試 | 對應 ts_lsp.c 修正 | 隨 ts_lsp.c 一起處理 |
| `src/discover/gitignore_core.c` | `.gitignore` 比對區分大小寫 | Windows 上不分大小寫（與 `core.ignorecase=true` 的 git 一致） | `Build/` 在 Windows 的 git 會忽略 `build/`，索引卻會收進去（以真實 git 驗證） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_gitignore.c` | — | 新增大小寫比對測試 | 對應 gitignore_core.c 修正（上游 2026-10 把比對引擎從 gitignore.c 搬到此檔，修正隨之移植） | 隨 gitignore_core.c 一起處理 |

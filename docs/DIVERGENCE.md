# 分岔登記表

本 fork 改動過的**上游持有檔案**，每個檔案一列。新增的檔案（如 `AGENTS.md`、`tools/*.py`）不算。

[`tools/check_divergence.py`](../tools/check_divergence.py) 會比對「自基準 commit（`tools/upstream_baseline.json`）起改過或刪過的上游檔案」與本表，不一致就非 0 退出（已接進 `dev_check.ps1` 與每週的 `upstream-check`）。

最後一欄寫成可執行的判準，同步上游時照做，不用重新評估。

| 上游檔案 | 上游原狀 | 本 fork 狀態 | 為什麼分岔 | 跟進上游時怎麼處理 |
|---|---|---|---|---|
| `README.md` | 英文 README | 改寫為繁中精簡入口 | 公開入口以繁中為主；原文保留在 `README.en.md` | 把上游新增的產品事實（工具數、語言數、安裝指令）併入本檔，不整份覆蓋 |
| `README.en.md` | 不存在（原文就是 `README.md`） | 上游 `README.md` 原文，第一行換成語言切換列 | 保留英文原文 | 用上游新版全文覆蓋，再把第一行換回語言切換列 |
| `.gitignore` | 上游忽略規則 | 尾端加 `# --- fork ---` 區塊：`!CHANGELOG.md` 與本機 gate 產物 | 上游忽略 `CHANGELOG.md`，但本 fork 的 `CHANGELOG.md` 需入版控；gate 會產生 `.venv/` 等檔 | 上游新增規則併在 fork 區塊之上；上游若自己處理同一項，刪掉重複行 |
| `src/cypher/cypher.c` | `cross_join_with_rels` 對 1 格緩衝區逐列寫入；WHERE 的 AND/OR/XOR 鏈與 UNION 分支無長度上限；交叉連接只擋 INT_MAX | 改用 `binding_out_append`；AND/OR/XOR 上限 4096、UNION 上限 64；交叉連接中間列數上限 25 萬 | heap overflow；數十萬個 AND 使評估遞迴 stack overflow；`MATCH (a) MATCH (b)` 可耗盡記憶體（皆有回歸測試） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/foundation/compat.h` | Windows `cbm_setenv` 先呼叫 `_putenv_s`，失敗就整個返回 | `_putenv_s` 失敗（ANSI 碼頁無法表示的字元，EILSEQ）不再中止，仍以寬字元 API 設定 | UTF-8 環境變數在非 UTF-8 碼頁的 Windows 上設定失敗 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/ui/http_server.c` | `/api/index` 以截斷後的路徑建索引、回應與 `/api/browse`、日誌輸出未完整跳脫 | 拒絕超過 job slot 的路徑；補跳脫；補一處 doc 洩漏 | 截斷可繞過工作區邊界；Windows 路徑的回應不是合法 JSON | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/watcher/watcher.c` | 用窄字元 `stat()` 處理 UTF-8 路徑；根目錄 ENOENT 一律視為已刪除 | 新增 `watcher_stat`（Windows 走寬字元 API）；磁碟機不存在（拔除的隨身碟、斷線的網路磁碟）或 UNC 路徑時視為「不確定」而非已刪除 | 中文路徑下 watcher 靜默失效；磁碟機拔除超過寬限期後索引資料庫被刪除 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/lsp/rust_cargo.c` | `members` 陣列遇到未加引號的項目無限迴圈 | 該輪未前進時強制前進一個字元 | 畸形 Cargo.toml 使索引 worker 卡死 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `internal/cbm/cbm.c` | Perl 巢狀過深的早退未記 `mark_done` | 補 `cbm_index_mark_done` | 正常略過被 supervisor 誤判為 crash 嫌疑 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `scripts/ci/new-protected-temp-root.ps1` | 以 `Set-Acl` 設定 ACL | 改用 `DirectoryInfo.SetAccessControl` | `Set-Acl` 需要 SeSecurityPrivilege，非管理員無法執行測試 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_cypher.c` | — | 新增 OPTIONAL MATCH 空標籤、長 AND 鏈、長 UNION 鏈測試；交叉連接上限斷言 | 對應 cypher.c 修正 | 隨 cypher.c 一起處理 |
| `tests/test_httpd.c` | — | 新增兩個 index 回歸測試 | 對應 http_server.c 修正 | 隨 http_server.c 一起處理 |
| `tests/test_watcher.c` | — | 新增非 ASCII 根目錄測試、磁碟機不存在不修剪測試 | 對應 watcher.c 修正 | 隨 watcher.c 一起處理 |
| `tests/test_rust_lsp.c` | — | 新增畸形 `members` 測試 | 對應 rust_cargo.c 修正 | 隨 rust_cargo.c 一起處理 |
| `tests/test_cli.c` | 讀取根目錄 `README.md` 驗證支援的 agent 清單 | 改讀 `README.en.md` | `README.md` 已改為繁中入口，上游原文在 `README.en.md` | 上游改讀哪個檔都跟著改回讀 `README.en.md` |
| `tests/test_language_count_contract.sh` | 檢查 `README.md` 的語言數 | 改檢查 `README.en.md` | 同上 | 同上 |
| `tests/test_vt_gate_policy_contract.sh` | 檢查 `README.md` 的 VirusTotal 政策文字 | 改檢查 `README.en.md` | 同上 | 同上 |
| `tests/test_smoke_fixture_contract.sh` | 以 `sys.platform != "win32"` 判斷是否為原生 Windows | 同時排除 `cygwin`、`msys` | MSYS2 的 python 回報 `cygwin`，Unix 專用檢查在 Windows 上誤跑 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_venue_parity_contract.sh` | `read_text()` 未指定編碼 | 指定 `encoding="utf-8"` | 預設碼頁（如 cp950）無法解碼 UTF-8 檔案 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_release_gate_chain_contract.sh` | `read_text()` 未指定編碼 | 指定 `encoding="utf-8"` | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_vt_release_notes_contract.sh` | `read_text()` 未指定編碼 | 指定 `encoding="utf-8"` | 同上 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/store/store.c` | `cbm_extract_like_hints` 把 `\w`、`\d` 當字面字母，也把 `?`、`*` 前的可省略字元算進必要字面 | `\` 加英數視為 meta；`? * {` 前一字元不納入；`)` 後接 `? * {` 時不產生任何 hint | hint 是 AND 過濾，錯誤 hint 會讓合法 regex 搜尋（如 `handle\w+`）漏掉結果 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_store_search.c` | — | 新增 hint 邊界案例斷言 | 對應 store.c 修正 | 隨 store.c 一起處理 |
| `src/graph_buffer/graph_buffer.c` | `cbm_gbuf_load_from_db` 不檢查 `sqlite3_step` 結束碼、不檢查 id 是否為負 | 讀取未以 SQLITE_DONE 結束就回傳失敗；負 id 不寫入重新對應表 | 讀取中途失敗會得到被截斷的圖並被後續寫回覆蓋資料庫；負 id 造成越界寫入（無法以測試重現，屬防禦性修正） | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_graph_buffer.c` | — | 新增負節點 id 測試 | 對應 graph_buffer.c 修正（修正前也通過，僅作冒煙測試） | 隨 graph_buffer.c 一起處理 |
| `scripts/setup-windows.ps1` | 下載 zip 後不驗證即解壓並設為 MCP 命令 | 下載 `checksums.txt` 並比對 SHA-256，不符即中止；暫存改用隨機目錄 | `irm \| iex` 推薦路徑沒有任何完整性檢查 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/foundation/compat_fs.c` | Windows `cbm_readdir` 遇到無法轉成 UTF-8 的檔名就結束整個目錄列舉 | 略過該項目並繼續列舉 | NTFS 允許孤立代理字元檔名；其後所有檔案從索引中靜默消失 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `tests/test_platform.c` | — | 新增孤立代理字元檔名的列舉測試 | 對應 compat_fs.c 修正 | 隨 compat_fs.c 一起處理 |
| `src/main.c` | `daemon --port=` 以 `atoi` 解析 | 改用 `strtol` 並檢查結尾與範圍 | `80abc` 被當成 80；溢位為未定義行為 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |
| `src/ui/httpd.c` | Windows 設定 `SO_EXCLUSIVEADDRUSE` 失敗時仍繼續 bind | 失敗即關閉 socket 並回傳 NULL | 失敗會退回一般 bind，其他本機使用者可搶佔連接埠 | 上游若自己修好，採用上游版本並刪本列；否則保留本 fork 的修正 |

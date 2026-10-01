# REVIEW

最新覆核：2026-10-02。範圍：產品 C 程式碼（`src/`、`internal/cbm/` 非 vendored）、安裝腳本、workflow、測試腳本。

## 方法與環境

- Windows 11 + MSYS2 CLANG64（clang 22.1），ASan＋UBSan 測試 runner。
- 9 個區塊各自獨立讀碼審查（foundation／store、mcp／cypher、pipeline／discover／watcher、cli／daemon／ui、internal/cbm 抽取器、腳本與 workflow、config 編輯器、store 其餘與 supervisor、extract／LSP 本體）。
- 每個發現我都對照原始碼驗證，並盡量用真實二進位或真實 git 實測。能重現的先寫會失敗的測試、確認失敗，再修，修後重跑；實測不成立的剔除。

## 測試結果

| | 通過 | 失敗 | 略過 |
|---|---:|---:|---:|
| 修正前（僅補齊環境） | 7726 | 78 | 68 |
| 現在（含同步上游 90 個提交後） | 8179 | 0 | 79 |

另外：`scripts/test.sh` 的全部契約步驟通過；產品二進位回歸（watchdog、worker、scope、字串白名單等）通過。

## 已修正（皆有回歸測試，除非註明）

**資料損毀／資料遺失**

| 問題 | 位置 |
|---|---|
| Hermes（PyYAML 預設）縮排式序列會被插到原項目之前而損毀設定檔 | `src/cli/config_yaml_edit.c` |
| 磁碟機拔除或網路磁碟斷線超過寬限期後，索引資料庫被刪除 | `src/watcher/watcher.c` |
| ADR 空區段取代時，新內容黏到下一個標題上（標題被改名） | `src/store/store.c` |
| 磁碟滿的短寫可能被當成功發佈；頁面偏移在 Windows 超過 2 GiB 寫錯位置（無自動測試） | `internal/cbm/sqlite_writer.c` |

**當機、卡死、資源耗盡**

| 問題 | 位置 |
|---|---|
| 30 萬個 `AND` 的查詢讓伺服器 stack overflow（ASan 實測崩潰）；UNION 無上限 | `src/cypher/cypher.c` |
| `MATCH (a) MATCH (b)` 交叉連接可耗盡記憶體（上限 25 萬列） | `src/cypher/cypher.c` |
| TS／JS 深度巢狀檔案：ASan 單元測試 600 層即崩潰；真實二進位 8000 層 25 秒 → 5 秒，30000 層超過 5 分鐘 → 105 秒 | `internal/cbm/lsp/ts_lsp.c` |
| 畸形 `Cargo.toml` 的 `members` 使索引無限迴圈 | `internal/cbm/lsp/rust_cargo.c` |
| OPTIONAL MATCH 起點無節點時 heap overflow（上游同期也修了，已採用上游版本） | `src/cypher/cypher.c` |

**Windows 行為錯誤**

| 問題 | 位置 |
|---|---|
| 設定檔原子取代遇到掃描程式暫時鎖檔（錯誤 1175）就放棄，安裝／解除安裝隨機失敗；修正前每輪有 1–5 個隨機失敗，修正後連跑 8 輪全過 | `src/foundation/compat_fs.c` 及四個設定檔編輯器 |
| 中文等非 ASCII 專案路徑下 watcher 靜默失效（窄字元 `stat`） | `src/watcher/watcher.c` |
| `cbm_readdir` 遇到無法轉換的檔名就結束列舉，其後檔案從索引消失 | `src/foundation/compat_fs.c` |
| `cbm_setenv` 在 ANSI 碼頁無法表示的字元上失敗 | `src/foundation/compat.h` |
| `.gitignore` 比對區分大小寫，與 Windows 上 `core.ignorecase=true` 的 git 不一致（已用真實 git 驗證） | `src/discover/gitignore.c` |
| Codex hooks 在安裝路徑含空白時永遠裝不起來（上游同期也修了，已採用） | `src/cli/config_toml_edit.c` |

**搜尋、HTTP 介面、安裝腳本**

| 問題 | 位置 |
|---|---|
| 合法 regex（`handle\w+`、`colou?r`）的搜尋漏結果（hint 計算錯誤） | `src/store/store.c` |
| `/api/index` 路徑被截斷可繞過工作區邊界；回應與日誌 JSON 未跳脫 | `src/ui/http_server.c` |
| `setup-windows.ps1` 下載後不驗證雜湊（實測：真檔通過，竄改／缺項／空檔皆拒絕） | `scripts/setup-windows.ps1` |
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

## 未結

- **C 前處理器巨集展開炸彈**：1 KB 的檔案，2^18 個 token 要 9.6 秒、2^22 個超過 120 秒不完成。simplecpp 沒有展開上限；它是 vendored 且受完整性檢查保護，不在此修。最終由 supervisor 的記憶體預算與 15 分鐘無進展逾時終止。
- **`daemon_ipc` 4 個測試**在本機以預設 `LOCALAPPDATA` 失敗：`AppData\Local` 的 ACL 含 AppContainer 與 `CodexSandboxUsers`，daemon 的祖先目錄安全檢查因此拒絕。把 `LOCALAPPDATA` 換到乾淨位置後 37 個全過，確認是環境而非程式；未動安全邏輯。
- 一個計時型測試（`daemon_ipc_windows_startup_retries_transient_rendezvous_reader`）在 24 個並行 job 下偶發失敗，單獨跑通過。
- 審查提出但未處理：`setup.sh`（非 Windows）無雜湊驗證；OOM 時未檢查 malloc 的多處；`cbm_max_file_bytes` 在 Windows 上限 2 GiB；LIKE 萬用字元未跳脫的範圍計數；sqlite_writer 的 B-tree 根頁回傳 0 未檢查；`cbm_count_*_scoped` 讀取失敗回 0；YAML 編輯器在 Windows 以目錄當鎖、崩潰後會殘留；TOML 多行陣列內以 `[` 開頭的行被當成表頭。
- 沒有逐行讀完：`store.c` 部分區段、多數 `extract_*.c` 細節、`pass_route_nodes.c` 後段。這些不能視為已排除。
- 上游持續前進：本次同步到 `7d4a12c`（2026-10-02）；之後又有 7 個提交、27 個 PR、6 個 issue 待審查（`upstream-check` 會持續提示）。

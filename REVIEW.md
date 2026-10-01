# REVIEW

最新覆核：2026-10-01。範圍：產品 C 程式碼（`src/`、`internal/cbm/` 非 vendored）、安裝腳本、workflow、測試腳本。

## 方法與環境

- Windows 11 + MSYS2 CLANG64（clang 22.1），ASan＋UBSan 測試 runner，`scripts/build.sh`、`scripts/test.sh`。
- 6 個區塊（foundation／store、mcp／cypher、pipeline／discover／watcher、cli／daemon／ui、internal/cbm、腳本與 workflow）各自獨立讀碼審查。
- 每個發現都由我對照原始碼驗證。能重現的先寫會失敗的測試，確認失敗後再修，修後重跑；審查誤判的降級或剔除。

## 測試結果

| | 通過 | 失敗 | 略過 |
|---|---:|---:|---:|
| 修正前（僅補齊環境） | 7726 | 78 | 68 |
| 現在 | 8110 | 4 | 75 |

剩下的 4 個失敗都在 `daemon_ipc`（見「未結」）。`scripts/test.sh` 的契約步驟 0r 起需要建立符號連結的權限，本機沒有，交給 CI。

## 已修正（皆有回歸測試，除非註明）

| 問題 | 位置 | 修正 |
|---|---|---|
| OPTIONAL MATCH 起點無節點時 heap overflow | `src/cypher/cypher.c` | 02c1a2b2 |
| 30 萬個 `AND` 的查詢讓伺服器 stack overflow（ASan 實測崩潰）；UNION 無上限 | `src/cypher/cypher.c` | 0cdd9b03（上限 4096／64） |
| `MATCH (a) MATCH (b)` 交叉連接可耗盡記憶體 | `src/cypher/cypher.c` | 0cdd9b03（上限 25 萬列） |
| 畸形 `Cargo.toml` 的 `members` 使索引無限迴圈 | `internal/cbm/lsp/rust_cargo.c` | 02c1a2b2 |
| 中文等非 ASCII 專案路徑下 watcher 靜默失效（窄字元 `stat`） | `src/watcher/watcher.c` | 02c1a2b2 |
| 磁碟機拔除／網路磁碟斷線超過寬限期後索引資料庫被刪除 | `src/watcher/watcher.c` | 本提交 |
| `cbm_setenv` 在 ANSI 碼頁無法表示的字元上失敗 | `src/foundation/compat.h` | 02c1a2b2 |
| `/api/index` 路徑被截斷可繞過工作區邊界；回應與日誌 JSON 未跳脫 | `src/ui/http_server.c` | 02c1a2b2 |
| `cbm_extract_like_hints` 讓合法 regex（`handle\w+`、`colou?r`）搜尋漏結果 | `src/store/store.c` | 0cdd9b03 |
| Windows `cbm_readdir` 遇到無法轉換的檔名就結束列舉，其後檔案消失 | `src/foundation/compat_fs.c` | 本提交 |
| `setup-windows.ps1` 下載後不驗證雜湊 | `scripts/setup-windows.ps1` | 0cdd9b03（實測：真檔通過，竄改／缺項／空檔皆拒絕） |
| `--port=80abc` 被當成 80（`atoi`） | `src/main.c` | 本提交（以真實二進位驗證，無單元測試） |
| `SO_EXCLUSIVEADDRUSE` 設定失敗仍 bind | `src/ui/httpd.c` | 本提交（無法觸發，無測試） |
| Perl 早退漏記 `mark_done`，被誤判為 crash 嫌疑 | `internal/cbm/cbm.c` | 02c1a2b2 |
| `graph_buffer` 讀取不檢查 `sqlite3_step` 結束碼、負 id | `src/graph_buffer/graph_buffer.c` | 0cdd9b03（防禦性；測試在修正前也通過，ASan 無法偵測該配置器的越界） |
| 非管理員無法建立測試用受保護暫存根（`Set-Acl` 需要 SeSecurityPrivilege） | `scripts/ci/new-protected-temp-root.ps1` | 02c1a2b2 |
| 測試腳本：`README.md` 改為繁中後讀取失敗、預設碼頁讀 UTF-8 失敗、MSYS2 python 被當成 Unix | `tests/*.sh`、`tests/test_cli.c` | 02c1a2b2 |

## 已驗證、不需修改

- Windows 二進位已啟用 `DYNAMIC_BASE`、`HIGH_ENTROPY_VA`、`NX_COMPAT`（`objdump -p` 實測，lld 預設）。
- cppcheck 對 `long` 與 `INT_MAX` 比較的警告：Windows 上 `long` 為 32 位元，比較多餘但語意正確。
- 審查說 watcher 會刪 DB 的成因（窄字元 `stat`）在此工具鏈實測回 `EILSEQ` 而非 `ENOENT`，不會刪除，只是失效；已修。

## 未結

- **`daemon_ipc` 4 個失敗**（`windows_legacy_bridge_*`、`windows_local_transition_*`、`endpoint_is_namespaced_*`、`relative_runtime_parent_*`）：本機 `AppData\Local` 的 ACL 含 AppContainer 與 `CodexSandboxUsers` 等額外主體，daemon 的祖先目錄安全檢查因此拒絕。未動安全邏輯；需在乾淨的 Windows 環境（CI）確認。
- 審查提出但未驗證或未處理：`extract_channels.c` 等處逆序走訪的 O(N²) 效能、`setup.sh` 無雜湊驗證與 Linux 資產名稱（非 Windows）、workflow 內 `${{ inputs.* }}` 直接插入 shell、`cbm_max_file_bytes` 在 Windows 上限 2 GiB、`.gitignore` 大小寫比對、OneDrive 雲端佔位檔可能被當成符號連結略過（本機 OneDrive 無此類檔案，無法重現）。
- `src/` 內未逐行讀完的部分：`store.c` 約六成、`index_supervisor.c`、多數 `extract_*.c`、LSP 各檔本體、`config_*_edit.c`。這些不能視為已排除。

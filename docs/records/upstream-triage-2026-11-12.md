# 上游同步（2026-11-12）

範圍：上游 `main` 由 `edfb824f` 前進到 `f5d57465`（123 個 commit：87 個非 merge、36 個 merge；82 檔，+1988385／−409；其中約 198 萬行是新增的 VB.NET 文法，`parser.c` 在 `internal/cbm/vendored` 與 `tools/` 各一份，約 97.7 萬行），加上 PR 2944–2959 與 issue 2941–2955 的新項目。`Upstream check` 的紅燈來源是 `edfb824f` 之後上游前進 123 個 commit（`check_upstream_updates.py --strict` 回報 123 個待審 commit、8 個新 PR、3 個新 issue）。

## 做法

以 `edfb824f` 為 base 三方合併：`git diff edfb824f upstream/main > up.patch`，`git apply -3 --index up.patch`。fork 與上游 history 無共同祖先（歷史已壓縮），所以不能用 `git merge`，改用 blob 級的三方套用。

套用結果：82 檔中 16 個是本 fork 也改過的上游檔案，其中 14 個乾淨合併；2 個衝突：

| 檔案 | 處理 |
|---|---|
| `README.md` | 保留繁中入口（衝突區是上游英文 README 的安裝段，整段不併入）；上游 `163 種語言` 的變動改寫進繁中版第 15 行（原寫 162）。 |
| `src/pipeline/registry.c` | 上游 `bbd09579` 把 `malloc` 改為 `cbm_alloc`（tracked allocator），仍未檢查回傳值。取上游的 `cbm_alloc`，保留本 fork 的 NULL 檢查（`cbm_alloc` 同樣可能回 NULL）。DIVERGENCE 該列改寫說明。 |

`README.en.md` 不在上游的 diff 中（它是 fork 檔）：依 DIVERGENCE 規則，改為上游新版 `README.md` 全文，第一行換成語言切換列（相對上一版只有 5 行改動）。

## 上游 commit 的實質內容（123 個）

- Swift 呼叫綁定與 callable identity（PR 2436 整批，`#2061`）：`extract_*.c`、`pass_*.c`、`registry.c` 大量變動。本 fork 沒有 Swift 相關的分岔列，乾淨套用。
- C# 接收者（`registry.c`，`receiver_chain_admits`、`CBM_LANG_CSHARP` 閘門）與 JS/TS 接收者綁定（`extract_calls.c`、`extract_imports.c`、`pipeline` 的 `same_module` 排除）。
- 新文法：VB.NET（PR 2948，`internal/cbm/vendored/grammars/vbnet`、`tools/tree-sitter-vbnet`），語言數 162 → 163。
- Watcher 新增 SVN 工作副本觀察（`src/watcher/svn_state.c`，PR 2715）與 `qsort` NULL 防護。
- daemon：只快取本行程的映像指紋（`45bac491`）、hook 截止時間的診斷（`9545ea4f`）。

## 併入的 PR（已在 `main`，本輪整批取用）

| PR | 內容 | 決定 |
|---:|---|---|
| 2947 | Swift：讀取呼叫點當時有效的綁定 | 採用（整批併入） |
| 2948 | 新增 VB.NET tree-sitter 文法 | 採用（vendored；`MANIFEST.md` 與 `vendored-checksums.txt` 隨上游更新） |
| 2950 | 測試：Unicode 專案名稱的 FQN 衝突防護 | 採用（測試） |
| 2951 | 測試：daemon 閒置維護探測的節奏 | 採用（測試） |

## 新 PR 分流（上游 PR 2944–2959）

| # | 標題（縮） | 狀態 | 決定 |
|---:|---|---|---|
| 2946 | Go：等待二進位驗證，不用截止時間 | OPEN、draft | 觀察；草稿，動 `pkg/go` wrapper 與 Windows 測試。觸發：轉為 ready 且併入後逐檔讀 diff |
| 2952 | 測試：Windows 乾淨的 MCP stdio 啟動守衛 | OPEN | 觀察；純 Windows 測試，有利本 fork。觸發：併入後直接採用 |
| 2957 | 文法：SQLite 遷移不被誤標為部分解析 | OPEN | 延後；`sql` 文法 vendored 替換（約 240 萬行 diff），合併前不評估。觸發：併入後以 `scripts/audit-license-provenance.py` 重新核對授權 |
| 2959 | 測試影響分析：CI 流程與工具鏈 | OPEN | 不採；改動上游 `pr.yml`、`_test.yml` 與 `docs/SMART_CI.md`，本 fork 的 CI 與上游不同（停用上游 PR 流程）。觸發：上游併入後再評估是否有值得的測試選擇邏輯 |

## 新 issue 分流（上游 issue 2942–2955）

| # | 標題（縮） | 狀態 | 決定 |
|---:|---|---|---|
| 2945 | Python：`Depends(fn)` 的 CALLS 邊在專案變大後消失（~14k 行） | OPEN | 觀察；有 `parsing/quality`、`windows` 標籤。尚無上游修正，無本地重現。觸發：上游 PR，或本機重現 |
| 2949 | Perl：跨檔方法解析與 accessor 型別（Moose、Mojo::Base） | OPEN | 延後；`enhancement` 標籤，功能性新增，本 fork 不急用。觸發：上游 PR |
| 2955 | Daemon 快取目錄不符錯誤訊息顯示 SHA-256 指紋而非實際路徑 | OPEN | 延後；只影響錯誤訊息文字，無上游修正。觸發：上游 PR，或本機實際遇到 |

PR 2944、2953、2954、2956、2958 不在 `gh pr list`、`gh issue list`（各取最新 60 筆，`--state all`）的結果中，本次未查證其原因。

## 驗證（2026-11-12，Windows 11、MSYS2 CLANG64、clang）

- 建置：同步前（`278b8eac`，以 `git worktree` 放在 `build/pre-sync`）與同步後的 `scripts/build.sh CC=clang CXX=clang++` 都 exit 0；test-runner 以 ASan＋UBSan 增量編譯，兩邊各一份。
- 執行環境：測試執行檔複製到受保護暫存根的 `bin/`（`scripts/ci/new-protected-temp-root.ps1 -ProtectDir <build\c>`）；`USERPROFILE`、`HOME`、`TEMP`、`TMP`、`TMPDIR`、`CBM_CI_TEMP_ROOT` 指到工作目錄下的 `build/verify-*`，`LOCALAPPDATA` 指到自建的空資料夾；測試後刪除。未碰 `~/.claude`、`~/.codex*`、`~/.cursor`、`C:\AgentData`。未啟動常駐服務。
- 全部 194 個 suite 的並行 harness（`scripts/run-tests-parallel.sh`）在兩邊都以 `taskkill could not prove process-tree cleanup` 中止（同步前 158 個、同步後 175 個 suite 有結果）。沒有結果的 suite，加上逾時（rc=124）的 `pipeline`、`doc_mentions`，共 48 個，改用同一個 test-runner 逐 suite 執行（`timeout 1800`，4 路並行，兩邊相同方式）。
- 結果：194 個 suite 中，168 個兩邊皆 rc=0。其餘 26 個見下表；`svn_state` 是上游新增的 suite，同步前不存在（同步後 16 passed）。

同步前後失敗對照（`failed` 為該 suite 的失敗數；`=` 表示失敗的測試名稱集合相同）：

| suite | 同步前 | 同步後 | 說明 |
|---|---:|---:|---|
| extraction | 2 | 1 | 兩次的失敗都是計時型測試（`extract_ts_many_decorated_methods_is_linear` 等）；同步後的失敗名稱相同，時間數字受負載影響 |
| daemon_frontend | 2 | 3 | **新增**：`daemon_frontend_idle_maintenance_probe_is_bounded`（上游 2951 的測試）在 fixture 啟動即失敗（`test_daemon_frontend.c:1652`）；同一 fixture 在同步前的 `monitor-join` 測試也因相同原因失敗，屬環境類，本機無法驗證其斷言 |
| index_format | 2 | 4 | **新增 3 個斷言**，來自上游新增的 `index_format_version_one_rebuilds`、`index_format_swift_collisions_and_stale_calls_rebuild`，失敗機制與同步前的 193、283 相同（未經過 full-reindex 路徑、`format_change_reindex` 日誌缺漏）；同步前的失敗已存在，原因未查明，本機無法證明新測試通過 |
| pipeline | 4 | 4 | 失敗名稱相同；通過數 454 → 482（上游新增測試） |
| runner_isolation、private_file_lock、project_lock、version_cohort、daemon_version、daemon_runtime、daemon_application、daemon_bootstrap、daemon_ipc、git_context、discover、pipeline_semantic_manifest_repro、index_resilience、doc_mentions、doc_mentions_rs、runtime_settings、httpd、integration、mcp、grammar_probe_f、incremental | 同 | 同 | 失敗名稱集合相同（ACL、daemon 行程與 fixture 啟動、計時等環境類；同步前即存在） |

判定：沒有發現新的程式碼回歸。新增的失敗都落在上游新增的測試上，而且與同步前既有的失敗機制相同，但它們在本機未通過，所以這幾條斷言本身沒有被驗證。

環境類失敗的數量比上一輪（同步前 5、後 7）多很多，主要集中在 daemon、IPC、version、runtime、discover 等 suite；兩邊相同，所以不影響比較，但也表示這一輪的測試訊號很弱。原因沒有查明，可能與本次的暫存目錄設定有關，未驗證。

## 驗證的未涵蓋範圍

- 未在乾淨的 CI 環境執行；`scripts/test.sh` 的完整預設流程（clean sanitizer build、契約步驟 0a–0t、prod-binary guard）沒有跑。
- 維護 gate：`tools/dev_check.ps1` exit 0（WINDOWS DEV CHECK GREEN）；`tools/check_upstream_updates.py --strict` exit 0（無待審 commit、PR、issue）；`tools/check_divergence.py` 92 對 92。
- 未執行：`make -f Makefile.cbm lint-format lint-no-suppress`（格式與 NOLINT 檢查）。
- 上游 Swift identity（#2061）與 VB.NET 文法的實際索引品質沒有另外評估，只有測試結果。
- 新增的上游測試：`daemon_frontend_idle_maintenance_probe_is_bounded`、`index_format_version_one_rebuilds`、`index_format_swift_collisions_and_stale_calls_rebuild` 在本機環境未通過，未驗證。

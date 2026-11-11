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
| 2961 | GDScript：vendored tree-sitter-gdscript 落後上游，合法的 Godot 4 程式回報 `parse_partial` | OPEN | 延後；文法 vendored 更新，本 fork 無 GDScript 分岔，超出本輪範圍。觸發：上游併入 vendored 文法更新，或本機以 Godot 專案重現。注意：本 issue 在第一次 `gh issue list` 時未出現，於寫入 commit 前的最後一次 `check_upstream_updates.py --strict` 才被列出，所以水位在後續 commit 才推進到 2961 |

PR 2944、2953、2954、2956、2958 不在 `gh pr list`、`gh issue list`（各取最新 60 筆，`--state all`）的結果中，本次未查證其原因。重新執行 `check_upstream_updates.py --strict` 時，PR 清單中 2959 以上沒有新項目，issue 清單中 2956 至 2960 沒有 issue，2961 是唯一新增項。

## 驗證（2026-11-12，Windows 11、MSYS2 CLANG64、clang，ASan＋UBSan test-runner）

### 第一輪（不採信）
第一輪的約 160 項失敗不能作為基準。原因已查明，見下。第一輪的失敗表已移除。

### 失敗原因（已查明）
兩個環境設定錯誤，同步前後都受影響：
1. 暫存目錄放在工作樹的 `build/` 下。`build/` 在 `.gitignore` 裡，測試的暫存 fixture 被 gitignore 規則排除，`discover`（9 項）與依 `TEMP` 建立 fixture 的 suite 因此失敗。
2. 受保護暫存根放在 `%TEMP%` 底下（`AppData\Local\Temp\...`）。daemon 的 endpoint 祖先目錄安全檢查拒絕這個路徑，`project_lock`、`daemon_version`、`private_file_lock` 等 suite 的 endpoint 建立失敗。

修正：受保護暫存根直接放在使用者家目錄下（`cbm-v2{pre,post}-<亂數>`，由 `scripts/ci/new-protected-temp-root.ps1 -ProtectDir <build\c>` 建立；該腳本須以 Windows PowerShell 執行）。`HOME`、`USERPROFILE`、`TEMP`、`TMP`、`TMPDIR`、`CBM_CI_TEMP_ROOT`、`LOCALAPPDATA`（根下的 `la/`）都指向該根，test-runner 副本放在根下的 `bin/`。測試後刪除。未碰真實的 `~/.claude`、`~/.codex*`、`~/.cursor`、`C:\AgentData`。

### 結果（同一方式，194 個 suite，每個 suite 獨立執行，4 路並行，`timeout 1800`）
- 同步前 `278b8eac`：191 個 suite rc=0；3 個非零：`extraction`（計時）、`subprocess`（記憶體門檻，負載相關）、`svn_state`（上游新增，同步前不存在）。
- 同步後 `a8b37c38`（程式碼與 `93b12a03` 相同，之後只改文件）：193 個 suite rc=0；1 個非零：`extraction`。
- 兩邊 `extraction` 的失敗都是 `extract_ts_many_decorated_methods_is_linear`，為計時斷言（20 秒上限，4 路負載下 25–31 秒）。單獨執行時，同步前與同步後都通過。
- `subprocess` 在同步前失敗（`loaded.rss >= idle.rss + 96 MiB`，負載相關）、同步後通過；無法歸因為同步的回歸或修正。
- 沒有任何 suite 是同步後失敗、同步前通過。

### 新增的上游測試（`CBM_TEST_ONLY` 單獨執行，同步後）
- `daemon_frontend_idle_maintenance_probe_is_bounded`：PASS。
- `index_format_version_one_rebuilds`：PASS。
- `index_format_swift_collisions_and_stale_calls_rebuild`：PASS。
三項也在完整執行中通過。前一輪的失敗是暫存目錄設定造成的。

### F1–F7 回歸測試
`mcp`（411 passed，含 F1 的 Windows 命令列與 U+2018–U+201B 測試）、`pipeline`（486 passed，含 F2 Rust crate roots）、`extraction`（553 passed，含 F3／F4 Rust use 樹與 inline mod）、`watcher`、`index_format`、`cypher` 全數通過。

### 維護 gate
- `tools/dev_check.ps1`：WINDOWS DEV CHECK GREEN。
- `tools/check_upstream_updates.py --strict`：exit 0。
- `tools/check_divergence.py`：92 對 92。
- `make -f Makefile.cbm lint-format lint-no-suppress`：exit 0（clang-format 與 NOLINT 檢查都通過）。

## 未涵蓋範圍
- 沒有跑 CI 完整預設流程：clean sanitizer build、契約步驟 0a–0t、prod-binary guard。
- 沒有評估 Swift identity、VB.NET 文法的實際索引品質，只有測試結果。

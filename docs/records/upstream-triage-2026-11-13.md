# 上游同步（2026-11-13）

範圍：上游 `main` 由 `f5d57465` 前進到 `a3e9c91a`（25 個 commit，其中 11 個 merge、14 個非 merge；70 檔，+2413192／−913456，其中約 240 萬行是 SQL 文法的 parser 與 sql 文法的 vendored 重新產生）。併入的 PR：2952、2957、2959、2962、2967、2968，另有一個較早編號的 PR 2427（`b3ec08da`，於基準之後才併入 main）。`check_upstream_updates.py` 的 PR 水位推進到 2972，issue 水位推進到 2964。

## 做法

fork 與上游沒有共同祖先（歷史已壓縮），所以不用 `git merge`。依 11-12 的方法：`git diff f5d57465 a3e9c91a` 產生 patch，再依每個檔案的內容分流：

1. 本 fork 與基準 `f5d57465` 內容相同的檔案（67 檔）：直接 `git checkout a3e9c91a -- <檔案>`，等同於三方合併後沒有衝突。之後逐檔比對 index 的 blob 與上游 blob，一致。
2. 本 fork 改過、且上游也改過的檔案（3 檔）：`git apply -3 --index`。結果：`src/pipeline/registry.c`、`tests/test_pipeline.c` 乾淨套用；`README.md` 衝突，取 fork 的繁中版（上游的測試範圍說明只屬於英文 README，見下）。
3. `README.en.md`：依 UPSTREAM 規則改為上游新版 `README.md` 全文，第一行換回語言切換列（相對上一版只有測試範圍那一句不同）。

第一次嘗試對整份 117 MB 的 patch 跑 `git apply -3 --index`，超過 25 分鐘未完成，已終止（exit 143）。終止時 index 有部分套用的內容，已由第 1 步的 checkout 覆蓋；終止後的 index 與上游逐檔比對，只剩下面第 2、3 步的檔案不同，符合預期。

## 衝突與分岔處理

| 檔案 | 處理 |
|---|---|
| `README.md` | 取 fork 的繁中版，不併入上游英文段落。上游改寫的「測試範圍」句只屬於英文 README，繁中版沒有該句。 |
| `README.en.md` | 上游新版全文＋語言切換列。 |
| `src/pipeline/registry.c` | 乾淨套用。獨立核對：fork 在後綴查詢（`cbm_registry_find_ending_with`）加的 `cbm_alloc` NULL 檢查保留（Swift 查詢的 NULL 檢查上游原本就有）；上游 PR 2968 的測試檔排序修正一併帶入。 |
| `tests/test_pipeline.c` | 乾淨套用；fork 的 Rust crate root 測試保留。 |
| `scripts/vendored-checksums.txt`、`internal/cbm/vendored/grammars/MANIFEST.md`、`THIRD_PARTY.md` | 上游版本。SQL 文法 `parser.c`、`scanner.c`、`tree_sitter/*.h` 與清單雜湊一致（blob SHA-256 核對）。 |

基準前進前，`tools/check_divergence.py` 把本次併入的上游檔案列為「有變動但未登記」，前進基準後回報 92 對 92，與登記表一致。本次沒有新增或刪除登記列，因為上游沒有自行修好 fork 登記的任何項目。

## 併入的 PR（已在 main）

| PR | 內容 | 決定 |
|---:|---|---|
| 2952 | 測試：Windows 乾淨的 MCP stdio 啟動守衛 | 採用（11-12 的條件「併入後直接採用」已成立）。 |
| 2957 | 文法：SQLite 遷移不被誤標為部分解析（SQL vendored 文法替換） | 採用（11-12 的條件「併入後以 `audit-license-provenance.py` 重新核對授權」已成立）。核對結果：`PROVENANCE AUDIT PASSED`；SQL 的 LICENSE 與上游逐位元組相同。 |
| 2959 | 測試影響分析：CI 流程與工具鏈 | 採用，**推翻 11-12 的「不採」**。理由：`pr.yml`、`_test.yml`、`docs/SMART_CI.md`、`scripts/test-impact/*`、`src/mcp/test_impact_engine_reach.c` 在本 fork 沒有分岔，不採則會留下只載入一半的 test-impact 腳本與測試，反而不一致。11-12 的觸發條件（併入後再評估）已成立。影響：上游的 PR 測試選擇只在上游的 PR 流程生效，本 fork 的 gate 仍是 `fork-ci.yml`。 |
| 2962 | CI：MSan 執行前降低 mmap ASLR 熵 | 採用（只改上游的 `_test.yml`）。 |
| 2967 | 守護程式：FreeBSD 以 `LOCAL_PEERCRED` 讀取對端 pid | 採用（`src/daemon/ipc.c` 無分岔）。 |
| 2968 | 登錄表：測試檔內的定義對正式呼叫端排序為測試 | 採用。修正上游 PR 2427 的回歸（Scala／Go 測試檔的符號被當成正式程式）。 |
| 2427 | 登錄表：候選排序改用抽取器的測試判定 | 隨 commit 帶入（編號低於水位，但合併時間在基準之後）。 |

## 未併入的 PR（OPEN，不移植）

| # | 標題（縮） | 決定 |
|---:|---|---|
| 2965 | 快取目錄不符的錯誤訊息改顯示實際路徑（修上游 issue 2955） | 延後；未併入，不移植。觸發：上游併入。 |
| 2966 | 貢獻指南改指向 `test_route_canon.c`（文件） | 延後；文件，未併入。觸發：上游併入（併入後可直接採用）。 |
| 2969 | 新增 VB6／VBA tree-sitter 文法 | 延後；新文法連同授權與 vendored 雜湊，未併入不評估。觸發：上游併入後以 `audit-license-provenance.py` 與 `vendored-checksums` 核對。 |
| 2970 | 測試影響：解析缺口只點名到達的函式、變數、巨集 | 延後；改動剛併入的 `test_impact_engine_reach.c`，未併入不評估。觸發：上游併入。 |
| 2971 | MCP：取消請求時停止快取掃描（refs 上游 issue 2796） | 延後；未併入。觸發：上游併入。 |
| 2972 | C#：本地函式內的呼叫歸屬到外層方法（修上游 issue 2964） | 延後；未併入。觸發：上游併入，併入後以本 fork 的 C# 回歸測試驗證。 |

## 未併入的 issue（OPEN）

| # | 標題（縮） | 決定 |
|---:|---|---|
| 2963 | MCP 錯誤回應加入結構化錯誤類型（`error` 物件） | 延後；跨 15 個工具的協定變更，本 fork 沒有需求，尚無上游修正。觸發：上游出現實作 PR，或本 fork 的 MCP 用戶端需要機器可讀的錯誤分類。 |
| 2964 | C#：本地函式內的呼叫歸屬到檔案節點 | 延後；修正已在 PR 2972（未併入）。觸發：PR 2972 併入。 |

`check_upstream_updates.py --strict` 在水位前進後回報無新的 commit、PR（2972 以上）與 issue（2964 以上）。

## 驗證

MSYS2 CLANG64（clang 22.1.8）建置 sanitized test runner（ASan＋UBSan，test_seams=1），依 11-12 的配方在受保護暫存根下逐 suite 獨立執行（194 個 suite，4 路並行），同步前（82cc304a）與同步後各跑一次：

| | 同步前 | 同步後 |
|---|---|---|
| 通過 suite | 192/194 | 192/194 |
| 非零 suite | `extraction`、`subprocess` | `extraction`、`stack_overflow_c` |

- 三個非零 suite 都是計時或負載門檻：`subprocess` 的 RSS 增量門檻、`stack_overflow_c` 的 `go_many_shadow_bindings_no_slowdown`（42.7 秒，上限 20 秒）、`extraction` 的 `extract_ts_many_decorated_methods_is_linear`（約 21.6–21.9 秒，上限 20 秒）。單獨重跑在同步前後都通過（`extraction` 該測試以 `CBM_TEST_ONLY` 各跑兩次）。結論：沒有同步造成的回歸。
- `scripts/test.sh` Step 0d2（soak harness 隔離）同步前後都通過。

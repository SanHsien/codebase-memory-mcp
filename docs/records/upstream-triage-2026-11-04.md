# 上游同步（2026-11-04）

範圍：上游 `main` 由 `83109dd` 前進到 `e0c7a1da`（684 個 commit，其中 241 個非 merge；243 檔，+41768／−6595，`vendored/mimalloc/` 45 檔）。前 507 個（到 `a5b248f4`）已在 [`upstream-triage-2026-11-02.md`](upstream-triage-2026-11-02.md) 逐筆分流，本輪照該表「依主題的處理方向」整批取用；其後 85 個非 merge commit 與 PR 2888–2906、issue 2895–2908 在下面分流。上游沒有發新 release。本 fork 不在上游留言或開 PR。

## 做法

以 `83109dd` 為 base 三方合併（`git diff 83109dd e0c7a1da | git apply --3way`），合併前對新增行做隱藏內容掃描（長空白、超過 400 字元單行、`eval`、長 base64、零寬與雙向字元）：命中只有巨集續行對齊空白與長 Markdown 段落，mimalloc 無命中。12 檔衝突，處理如下；其餘 231 檔乾淨套用。

| 檔案 | 處理 |
|---|---|
| `README.md` | 保留繁中版，併入外掛市集安裝與 Windows 疑難排解連結；`README.en.md` 換成上游全文 |
| `src/foundation/compat.h` | 採用上游：`_wputenv_s` 寬字元更新已涵蓋本 fork 的 EILSEQ 修正，刪分岔列 |
| `src/cli/config_yaml_edit.c` | 上游新增 `parent: {}` 路徑（PR 2896）；本 fork 的縮排式序列 fail-closed 檢查移到它之前，否則該路徑會以錯誤標記為終點掃描越界。新增防護測試 `config_yaml_edit_indentless_sequence_under_empty_parent_is_refused`：拿掉檢查重跑仍通過，因為 hook 路徑只有兩層，`parent: {}` 只會出現在第 0 層，而第 0 層的映射範圍驗證先拒絕了欄 0 的 dash；越界目前走不到，檢查與測試是防日後路徑變深時的退化 |
| `internal/cbm/extract_defs.c`、`extract_unified.c`、`extract_calls.c`、`helpers.h` | Elixir：`when` 守衛拆解與同名子句合併採用上游（`cbm_elixir_def_head_unwrap_guard`、`fold_elixir_clause`），刪本 fork 的 `cbm_elixir_unwrap_guard`；保留本 fork 的模組限定 QN、`defimpl`／`defprotocol` 與祖先回溯上限（`WalkState`）。YAML 走訪保留本 fork 的線性版本（上游 issue 2388 只把固定堆疊改成可成長，本 fork 另修了平方時間）；作用域框架併入上游的 `enclosing_class_node` |
| `internal/cbm/lsp/cs_lsp.c` | 兩邊並存：本 fork 的深度上限與上游的 `CS_QUALIFIER_MAX_DEPTH` |
| `tests/test_*.c`（cs_lsp、extraction、go_lsp、pipeline、store_nodes） | 兩邊新增的測試並存 |

上游 commit 自帶測試，視為採用時的失敗測試。本 fork 自己的合併調整：`config_yaml_edit.c` 一處（見上，測試不是紅燈，原因已寫明）；另外兩個本 fork 測試跟著上游改過的簽名補參數（`cbm_pipeline_build_semantic_manifest` 多了 `const cbm_pipeline_t *`、`cbm_run_go_lsp_cross` 多了 `rel_path`，皆傳 `NULL`），由測試建置失敗發現。

## `a5b248f4..e0c7a1da` 新 commit（85 個非 merge）

全部隨本輪取用，依主題：

| 主題 | 代表 commit | 備註 |
|---|---|---|
| 索引 worker 資源看門狗、低 CPU 優先權 | `de161526`、`1e4751d3`、`349d80c6`（PR 2838）、`435a88be`、`7a1bee25`、`c78c077a` | 2026-11-02 表中的條件式 PR 2838 已合併 |
| `ignore_worktrees` 與 worktree 偵測 | `16698e81`、`00710073`、`72f1a52d`、`51a18c7b`、`394faaac`、`141b25af`、`77310d5f` | 預設 false，不改現有行為 |
| Ruby per-file resolver | `e9f87efe` 起的 `ruby-lsp` 系列 | |
| Python／Swift／PHP／Next.js 抽取 | `1b64c50c`、`05471904`、`71ab0154`、`f200d747`、`09dc36d2`、`fa331ed5` | |
| daemon／MCP 復原與新鮮度 | `358d6e76`、`789c60c8`、`bf30a1aa`、`8bd9d37b`、`a0d0521f` | |
| Hermes `hooks: {}` | `e8f0f4a2`（PR 2896） | 與本 fork 的 YAML 分岔合併，見上 |
| 安裝程式 Android／Termux 防呆 | `ed78ff05`、`8b2b0d76` | |
| 測試、benchmark、CI、actions 版本 | `51b48f11` 起的 benchmark 系列、`1efa00ee`、`11f20c57`、`f683b856`（PR 2878、2879） | fork 停用的 workflow 只帶檔案，不重新啟用 |

## PR（2888–2909）

| # | 標題（縮） | 狀態 | 決定 |
|---:|---|---|---|
| 2888 | 包在呼叫裡的 handler 保留 HANDLES | OPEN | 條件式；觸發：上游合併（對應 issue 2883） |
| 2896 | Hermes 編輯器接受 `hooks: {}` | MERGED | 本輪取用（`e8f0f4a2`） |
| 2898 | MCP 安裝保留 Codex 工具核准表 | OPEN | 不採；本 fork 不用 Codex 安裝流程 |
| 2900 | TS 經 `index.ts` 轉出鏈 | OPEN | 條件式；觸發：上游合併 |
| 2903 | 控制檔走訪略過名為 `nul` 的檔案 | OPEN | 條件式：本 fork 已有同一修正（`pipeline_incremental.c` 分岔列）；觸發：上游合併後改採上游並刪分岔列 |
| 2905 | Retrofit 介面方法產生 HTTP_CALLS | OPEN | 條件式；觸發：上游合併 |
| 2906 | 安裝 CLI 支援 agentty | OPEN | 不採；本 fork 不用該 agent |
| 2909 | 限制跨檔準備階段的記憶體 | OPEN（推送後出現） | 條件式；與 issue 2908 的大檔記憶體問題相關；觸發：上游合併 |

2026-11-02 表的條件式：2838 已合併並取用；2878、2879 已合併並取用（只動檔案）；2840、2885、2886、2887 仍 OPEN，決定不變。

## Issue（2895–2908）

| # | 標題（縮） | 狀態 | 決定 |
|---:|---|---|---|
| 2899 | macOS 系統 regcomp 超過 5117 位元組回 REG_ESPACE | OPEN | 不採；本 fork 主環境是 Windows |
| 2907 | Lens `/api/layout` 間歇 500（Linux／WSL2） | OPEN | 不採；尚無修正 |
| 2908 | 大型 CSV 自動索引耗盡記憶體（Windows，v0.11.0） | OPEN | 與主環境相關；本輪的 worker 資源看門狗（`index_max_rss_mb`）可設上限；觸發：上游出現針對 CSV 的修正 |

2026-11-02 表中 OPEN 的 issue 狀態未變（11-04 查詢）。issue 2865（mimalloc 3.5.3 在 MinGW）：本輪在 Windows 11 以 MSYS2 CLANG64 建置的產品執行檔可正常啟動（`--version`、`--help` exit 0），測試執行檔跑完下面的 suite 也沒有 loader 崩潰；這只涵蓋 CLANG64，不涵蓋上游發佈的 MinGW 二進位。

## 驗證（2026-11-04，Windows 11、MSYS2 CLANG64）

- `scripts/build.sh` 建置產品執行檔成功；`make -f Makefile.cbm lint-format` 綠。
- 測試執行檔照 [`DEVELOPMENT.md`](../DEVELOPMENT.md) 複製到受保護資料夾、用受保護暫存根，先逐一跑 26 個受影響 suite（exit 0），再不帶參數跑完整的全部 suite：10236 passed、110 skipped、0 failed，exit 0。
- 第一次在 `build\c` 直接跑時 cli 有 4 個 `cli_install_*` 失敗（`acl-grants-cross-account-mutation to S-1-5-11`），是文件已記載的目錄權限問題，換到受保護資料夾即通過；上游的兩個 Elixir 守衛測試因本 fork 的模組限定 QN 失敗，已改期望值並登記。

觸發條件：`Upstream check` 轉 failure，或上面的條件式 PR 合併。

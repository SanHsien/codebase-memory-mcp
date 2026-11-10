# 上游同步（2026-11-10）

範圍：上游 `main` 由 `e0c7a1da` 前進到 `edfb824f`（85 個 commit，其中 36 個非 merge；54 檔，+4347／−286），加上 PR 2910–2943 與 issue 2909–2941。多數 commit 的日期早於基準（8 月至 10 月），因為它們在功能分支上完成、11-05 之後才以 15 個 PR 併入 `main`。上游沒有發新 release（最新仍是 v0.12.1）。本 fork 不在上游留言或開 PR。

## 做法

以 `e0c7a1da` 為 base 三方合併（`git diff e0c7a1da edfb824f | git apply --3way`）。合併前對新增行做隱藏內容掃描（超過 400 字元單行、行內 40 個以上連續空白、`eval`／`new Function(`、60 字元以上 base64 樣式、零寬與雙向控制字元）：命中只有 C 程式的參數對齊空白，以及 `README.md`、`docs/index.html`、`docs/llms.txt`、`pkg/npm/README.md` 的長段落（agent 數量說明）；`eval`、base64、零寬與雙向字元零命中。三個 open PR（2909、2914、2915）的 diff 同樣掃過，無命中。

3 個檔案衝突，其餘乾淨套用：

| 檔案 | 處理 |
|---|---|
| `README.md` | 保留繁中版；上游的 45→46 種 client 與 `--no-hooks` 說明併入繁中版（一句）；`README.en.md` 換成上游全文（第一行換回語言切換列），相對上一版 `README.en.md` 只有上游那 7 行新增、4 行刪除 |
| `internal/cbm/extract_defs.c` | 衝突在 `extract_decorators`／`extract_route_from_decorators` 的呼叫：本 fork 已改吃 `CBMExtractCtx`，上游同處加入 Retrofit 的 `retrofit_client_route`。保留本 fork 的簽名，帶入上游的 Retrofit 區塊 |
| `tests/test_cli.c` | 上游的 agentty 斷言（46 個 client、registry 查詢）帶入，讀取檔案維持 `README.en.md`（`DIVERGENCE.md` 既有列） |

另一處：`src/pipeline/pipeline_incremental.c` 與 `tests/test_pipeline.c` 的 `nul` 檔案修正。上游 PR 2903 修的是同一個問題（上游 issue 2646），本 fork 11-04 的分流列為「合併後改採上游並刪分岔列」。`git apply --3way` 沒有報衝突，卻把兩份修正疊在一起，所以兩個檔案直接改成上游版本（`git diff edfb824f -- <檔案>` 無輸出），刪 `DIVERGENCE.md` 中對應的兩列與本 fork 自己的 `nul` 測試（上游有兩個同性質測試）。

## 併入的 PR（15 個）

| PR | 內容 | 決定與驗證 |
|---:|---|---|
| 1862 | `install --no-hooks`／`--hooks` 並持久化（`install_hooks` 設定） | 採用；預設仍是啟用 hook。`--no-hooks`／`--hooks` 在本機 Windows 未驗證：其新測試被 ACL 環境問題擋住（見驗證段） |
| 2205 | `daemon start` 不再重新啟用被停用的 UI（`cbm_ui_config_daemon_start_ui`） | 採用；`ui` suite 帶測試 |
| 2667 | Java：已證明為外部型別的接收者不做名稱猜測 | 採用；新增 `java_typed_calls` suite（`Makefile.cbm` 一處登記，`tests/test_main.c` 兩行） |
| 2749 | POSIX 無範圍搜尋改用 argv 階段（不經 shell） | 採用；僅 `#ifndef _WIN32`，Windows 不受影響，也無法在本機重現 |
| 2840 | watcher 依上次成功重建的耗時拉長下次輪詢（上限 5 分鐘） | 採用；本 fork 的 `watcher.c` 分岔（磁碟機不存在視為不確定）在不同區段，乾淨套用 |
| 2903 | 控制檔走訪略過名為 `nul` 的檔案 | 採用上游版本，刪分岔（見上） |
| 2905 | Retrofit 介面方法產生 HTTP_CALLS | 採用；與本 fork 的 `extract_defs.c` 簽名分岔合併（見上） |
| 2906 | 安裝 CLI 支援 agentty | 採用。11-04 的決定是「不採」（本 fork 不用該 agent），但它已併入 `main` 且與 `cli.c`、`agent_clients.c` 同檔，整批取用避免分岔（同 11-02 對 Codex／OpenCode 的處理） |
| 2911 | FreeBSD 建置 workflow | 採用檔案；只有 `workflow_dispatch`、唯讀權限，不是排程，不需停用 |
| 2913 | `/api/layout` 的 JSON 對損壞的 UTF-8 也可解析 | 採用；`ESCAPE_UNICODE` 旗標，`ui` suite 帶測試 |
| 2927、2928、2929 | codeql-action、setup-node、cosign-installer 的 SHA 釘選升級 | 採用；只動上游的 workflow，本 fork 自己的 workflow（`fork-ci`、`dependency-freshness`、`upstream-check`）不受影響 |
| 2932 | Rust 群組 `use a::{B, C}` 展開（issue 2922 已關閉） | 採用；`extraction`、`pipeline` suite 帶測試 |
| 2943 | Rust：`use` 路徑在所屬 crate 內解析 | 採用；`pipeline` suite 帶測試 |

Windows 相關項目逐筆讀過 diff：`0e8fc16a`（Windows 上略過 Gemini coverage hook 的清除，避免 Windows 專屬路徑錯誤）只改一個 `#ifndef _WIN32` 防護；`79a57341`（`nul`）見上；其餘 `fix` commit 的變更範圍都在上表。

## 仍 OPEN 的 PR

| # | 標題（縮） | head | 決定 |
|---:|---|---|---|
| 2809 | 隔離驗證路徑與空資料防護 | `678bc584` | 條件式不變；觸發：上游合併 |
| 2909 | 限制跨檔準備階段的記憶體（issue 2908 相關） | `ae3c3e11` | 條件式不變；觸發：上游合併。diff 改 `pass_lsp_cross.c`、`java_lsp.c`、`pipeline.c`，與本輪併入的 PR 2667 同檔，合併後會需要重新對版 |
| 2914 | `snapshot: true` 的證據封套（+692／−88，動 `mcp.c`） | `eec715e6` | 不採；選擇性新功能，本 fork 未用。觸發：上游合併 |
| 2915 | `detect_changes` 的 `doc_references` 範圍（+972／−11，動 `mcp.c`） | `022d4b4e` | 不採；同上。觸發：上游合併 |
| 2885、2886、2887、2888、2898、2900 | 11-02 與 11-04 表中的條件式 PR | — | 狀態仍是 OPEN，決定不變 |

## Issue（2909–2941）

| # | 標題（縮） | 狀態 | 決定 |
|---:|---|---|---|
| 2916 | Linux：Landlock 拒絕讀取 `/` 時在 MCP 初始化前退出 | OPEN | 不採；Linux 專屬，尚無修正 |
| 2920 | CFML vendored 文法整檔 `parse_unusable` | OPEN | 延後；在 vendored 文法內，不在本 fork 修改範圍；觸發：上游更新 vendored 文法 |
| 2922 | Rust 群組 `use` 變成一個名為 `{B, C}` 的 import | CLOSED | 已由 PR 2932 修好並隨本輪取用 |
| 2923 | Rust：`use std::iter::once` 被連到專案內名為 `iter` 的定義 | OPEN | 延後；觸發：上游出現修正 |
| 2924 | 同名定義超過 256 個時，限定呼叫不產生 CALLS 邊 | OPEN | 延後；觸發：上游出現修正 |
| 2925 | `compare_graphs` 的節點識別含專案名 | OPEN | 不採；選擇性功能 |
| 2926 | Rust：`macro_rules!` 內的呼叫沒有 CALLS 邊 | OPEN | 延後；觸發：上游出現修正 |
| 2930 | Rust：模組限定呼叫 `launchd::status()` 綁到呼叫者自己的 `status` | OPEN | 延後；與 PR 2943 同領域但標題與修正範圍不同，觸發：上游出現修正 |
| 2931 | 重開機後 `st_dev` 變動，daemon 永遠起不來（持久 `/tmp`） | OPEN | 不採；依賴 POSIX socket 身分，Windows 不適用 |
| 2933 | PowerShell 數值倍率 `1KB`、Tailwind `@apply` 括號值被標 `parse_partial` | OPEN | 延後；文法層級；觸發：上游出現修正 |
| 2934 | PostgreSQL：`%ROWTYPE`、`PERFORM` 列鎖、`SELECT INTO FOR SHARE` 的解析缺口 | OPEN | 延後；同上 |
| 2935 | INI：多行值報 `parse_partial` | OPEN | 延後；同上 |
| 2936 | 安裝跳過所有非 Codex client 的過期 `SKILL.md` | OPEN | 延後；觸發：上游出現修正（影響安裝流程，合併時一併檢視） |
| 2938 | 原生 0.12.1 的 `artifact.json` 在 worktree 首次索引後缺 `reconcile_basis` | CLOSED | 查不到對應的 `main` 修正 commit（`git log upstream/main -S reconcile_basis` 在基準之後無新增）；不採，記錄關閉狀態 |
| 2941 | UTF-16／帶 BOM 的 `requirements.txt` 被索引成一個假 Package | OPEN | 延後；Windows PowerShell 的重新導向會產生 UTF-16，與主環境相關；觸發：上游出現修正，或本機實際遇到 |

以上 issue 狀態與標籤是 2026-11-10 的 `gh issue list` 結果；內文未逐篇讀（沒有可合併的程式碼）。

## 驗證（2026-11-10，Windows 11、MSYS2 CLANG64）

先紅後綠（`scripts/test.sh --suites ...`，MSYS2 CLANG64，HOME／USERPROFILE／LOCALAPPDATA 指到暫存目錄，未註冊任何 MCP 伺服器）：

- 紅一：只還原 `src/`、`internal/`（保留上游測試）重建，測試執行檔編譯失敗，缺少新 API：`cbm_pipeline_java_external_target`、`cbm_watcher_success_delay_ms`、`CBM_CONFIG_INSTALL_HOOKS`、`CBM_AGENT_CLIENT_AGENTTY`、`cbm_ui_config_daemon_start_ui` 等。
- 紅二：只還原 `extract_imports.c`、`pass_pkgmap.c`、`layout3d.c`、`extract_defs.c`，跑 `pipeline,extraction,ui,edge_types_probe`：1139 passed、7 failed、2 skipped。失敗為 `rust_imports`（`test_extraction.c:5799`）、`test_pipeline.c:12924`、`test_pipeline.c:13369`（兩次）、`layout_json_invalid_utf8_roundtrip`（`test_ui.c:721`）、`test_edge_types_probe.c:1251` 與 `:1285`（Retrofit HTTP_CALLS）。
- 綠：合併後重建，跑 `cli,mcp,watcher,pipeline,extraction,ui,parallel,agent_clients,edge_types_probe,java_typed_calls`：2149 passed、10 failed、35 skipped。上列 7 項紅燈全數轉綠，`java_typed_calls`、`watcher`、`agent_clients`、`parallel`、`pipeline`、`extraction`、`edge_types_probe` 無失敗。
- 剩下 10 個失敗都在環境類，未歸因到合併內容：`cli` 6 個（`cli_install_*` 回 `acl-grants-cross-account-mutation`，即 `DEVELOPMENT.md` 記載的 `build\c` 目錄權限問題）、`mcp` 1 個（`test_mcp.c:20171`，`detect_changes` 測試 impact 決定為 `run_all`，即 `DEVELOPMENT.md` 記載的 MSYS `/tmp` 問題）、`ui` 3 個（`atlas_history_*`、`atlas_selection_impact_git_*`，是 git 子行程的測試 fixture，與本輪動到的 `config.c`、`layout3d.c` 無關）。依文件改用受保護資料夾重跑 `cli mcp ui`，在暫存目錄的環境下失敗更多（62 個），沒有得到乾淨的結果；這 10 個**沒有**在合併前的樹上對照過，所以只能說「沒有證據指向合併」，不是「已證明與合併無關」。完整全部 suite 本輪沒跑（11-04 跑過 10236 passed）。
- 事後對照（審查者在合併前後的樹上各跑 cli、mcp、ui）：失敗數 5 → 7；多出的 2 項是 PR 1862 的新測試 `cli_no_hooks_removes_codex_pretool_hook`、`cli_no_hooks_removes_augment_owned_scripts`，同為 `acl-grants-cross-account-mutation` 環境問題；既有測試無回歸；上面 ui 的 3 項（atlas_*）是前次執行環境特有，乾淨環境兩次都通過。上一點「沒有對照過」以本點為準。
- `tools/dev_check.ps1`：exit 0（compileall、ruff、pytest、連結、分岔登記：87 個上游檔案分岔、87 個已登記）；`python tools/check_upstream_updates.py --strict`：exit 0；`git diff --check` 無輸出。
- 測試中沒有寫死會隨日期失效的值（新增測試以相對時間或固定的假時鐘 `cbm_watcher_test_set_clock_ns` 取值）。

觸發條件：`Upstream check` 轉 failure，或上面的條件式 PR（2809、2909、2914、2915）合併。

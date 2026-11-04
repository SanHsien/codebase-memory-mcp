# 決策紀錄

- **上游基準**：`DeusData/codebase-memory-mcp` `main`（MIT）。同步規則見 [`UPSTREAM.md`](UPSTREAM.md)。
- **只留 `main` 分支與最新的上游版本 tag／release**（目前 `v0.12.1`）：release 資產鏡像自上游並核對雜湊，tag 指向本 fork 同步到該版本的提交；上游發新版時換掉舊的。上游的其他分支與舊 tag 不留在 fork；名稱與 SHA 存於 [`records/`](records/)，上游 repo 仍保有。
- **不刪非 Windows 的程式碼與文件**：本專案要能索引各平台、各語言的專案，且發佈、測試與 CI 相互引用這些檔案。「Windows 為主」由文件、CI 與 gate 表達。
- **動 C 原始碼的規則**：先寫會失敗的測試、確認失敗再修；本機以 MSYS2 CLANG64 建置與跑相關 suite。
- **README**：上游英文原文移到 `README.en.md`，另寫繁中 `README.md`，避免每次同步整檔衝突。
- **換行**：不加全域 `eol=lf`，上游有依賴 CRLF 的測試語料。
- **不啟用 `scripts/hooks`**：pre-commit 需要 clang／cppcheck／make。
- **commit 加 `-s`**：上游 DCO workflow 要求 `Signed-off-by`。
- **停用上游的排程與發佈 workflow**：`nightly-soak`、`cache-warm`、`scorecard`、`stale`、`pages`、`release`、`issue-labeler`、`label-actions`、`pr-acknowledgement`；可用 `gh workflow enable` 復原。
- **上游 issue／PR 只在本 repo 分流**，不在上游留言或開 PR：見 [`records/upstream-triage-2026-09-29.md`](records/upstream-triage-2026-09-29.md)（全量）、[`records/upstream-triage-2026-10-07.md`](records/upstream-triage-2026-10-07.md)、[`records/upstream-triage-2026-10-16.md`](records/upstream-triage-2026-10-16.md)、[`records/upstream-triage-2026-10-24.md`](records/upstream-triage-2026-10-24.md)、[`records/upstream-triage-2026-10-25.md`](records/upstream-triage-2026-10-25.md)、[`records/upstream-triage-2026-10-26.md`](records/upstream-triage-2026-10-26.md)、[`records/upstream-triage-2026-10-28.md`](records/upstream-triage-2026-10-28.md)、[`records/upstream-triage-2026-11-02.md`](records/upstream-triage-2026-11-02.md)、[`records/upstream-triage-2026-11-04.md`](records/upstream-triage-2026-11-04.md)（增量）。
- **上游自己修好的問題採用上游版本**：2026-10-07 同步時，兩個 setup 腳本改用上游版本（上游改由 `install.sh`／`install.ps1` 強制驗證雜湊）；2026-10-16 同步時，作用域名稱索引、呼叫歸屬、Cypher 長鏈與 UNION、`/api/index` 跳脫、scoop 版本規則改用上游版本；2026-10-24 同步時，watcher 的 UTF-8 路徑、Rust 泛型 impl 的 self 型別改用上游版本（細節見該次分流紀錄）；2026-10-26 同步時，SvelteKit HANDLES 邊屬性的跳脫改用上游的 `sveltekit_handles_props`；2026-10-28 同步時，Python 遞迴 lambda 的重入保護（PR 2422）與 SQLite 寫入器 64 位元頁面偏移（PR 2622）改用上游版本。2026-11-04 同步時，Windows `cbm_setenv` 改用上游的 `_wputenv_s`，Elixir `when` 守衛拆解改用上游的 `cbm_elixir_def_head_unwrap_guard`。
- **提交訊息不寫 `#編號`**：fork 沒有開 issue，`#2655` 這類寫法會被 GitHub 連到上游，在上游 issue 留下反向連結；改寫成「上游 issue 2655」。
- **2026-11-04 整批三方合併上游 `83109dd..e0c7a1da`（684 個 commit，243 檔，+41768／−6595）**：11-02 只分流的 507 個 commit 加其後 85 個非 merge commit 一次合併，不單筆挑（commit 互相依賴）；12 檔衝突，本 fork 的修正在上游自己修好處改用上游版本、其餘保留並登記在 `DIVERGENCE.md`；commit／PR／issue 水位推進到 `e0c7a1da`／2906／2908。驗證：MSYS2 CLANG64 建置、完整 C 測試全綠（10236 passed、110 skipped）、`dev_check.ps1` 綠。逐筆結論與驗證細節見分流紀錄。

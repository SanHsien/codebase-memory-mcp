# 路線圖

> 本 repo 後續工作的單一真相源。做完一項，就在同一個 commit 刪掉那一列；新工作只加在這裡。
> 現況段的數字是 2026-10-29 依 `main` `399cf17bcf1a` 查的快照，開工前先重查 GitHub。

## 現況

`codebase-memory-mcp` 是 `DeusData/codebase-memory-mcp` 的維護 fork：本機程式碼知識圖譜 MCP 伺服器，產品本體以上游為準，本 fork 只加維護層（Windows 維護 gate、上游追蹤與分岔登記、繁中 README；`FORK.md:1-8`）。最近 release 是 v0.12.1（2026-10-23，鏡像自上游 v0.12.1）。
健康：open PR 0、Dependabot 與 code scanning 告警 0，`main` 上 CodeQL SAST、DCO、Fork CI、Dependency freshness、Upstream check、test-impact-artifact 最近一次皆 success；issues 在 GitHub 上已停用，open issue 數是「未查到」，不是 0（2026-10-29 `gh` 查詢）。
上游審查水位在 commit `83109dd`、PR 2817、issue 2816（`tools/upstream_baseline.json`）；與上游「No common ancestor」是已知事實，同步用三方合併而非 `git merge` 上游歷史。上游 `main` 在 2026-10-28T17:16Z 仍有 push，快照之後是否出現未審項目「未查到」，以 `python tools/check_upstream_updates.py --strict` 為準。

## 近期：可以直接動手

| 項目 | 為什麼 | 驗收方式 | 建議執行者 | 證據 |
| --- | --- | --- | --- | --- |
| 查明 `detect_changes_scope_tests_answers_test_impact` 為何失敗 | 2026-10-28 同步後跑受影響 12 個 suite，有 5 個失敗且在合併前就失敗；其中 4 個 `cli_install_*` 已知是本機 ACL 環境問題，這 1 個沒有歸因，可能是真實缺陷也可能是環境 | 在 MSYS2 CLANG64（ASan＋UBSan）重跑該測試並得出根因；若是缺陷，先有重現測試再修並更新 `REVIEW.md`「未結」；若是環境，把條件寫進 `docs/DEVELOPMENT.md` 與 `REVIEW.md`「未結」 | Claude Opus | 深掃報告（2026-10-28）「追加：上游同步」驗證段；`REVIEW.md:150-` 未結段 |
| 讓 Dependency freshness 的報告在檢查失敗時仍進 step summary，並修正含 UNKNOWN 列時的措辭 | 檢查器 exit 1 時後續步驟不跑，報告進不了 summary；含 UNKNOWN 列時「N dependencies are behind」措辭不精確，會把查不到說成落後 | 用 `tools/tests/test_dependency_freshness.py` 新增測試：含 UNKNOWN 列的報告不寫成 behind；手動觸發 `dependency-freshness.yml`，兩種情況（全新鮮、有落後）summary 都有報告；`pwsh -NoProfile -File tools\dev_check.ps1` 綠 | Claude Sonnet | `.github/workflows/dependency-freshness.yml:55-84`；`tools/check_dependency_freshness.py:206`；`f348e023` |
| 讓 `tools/tests/test_links.py` 被強制中止時不在 repo 內留下暫存檔 | 測試在 `tools/tests/` 寫 `_untracked_link_target.md`，中途被殺會殘留，之後連結檢查與 `git status` 都被汙染 | 暫存檔改寫到 `tmp_path` 之類 repo 外位置（仍要測到「存在但未入版控」），或啟動時清掉殘留；測試仍能證明未入版控檔案被判為壞連結；`dev_check.ps1` 綠 | Claude Haiku | `tools/tests/test_links.py:26-37`；`69640114` |
| 文件收尾三處 | `REVIEW.md` 開頭寫「最新覆核：2026-10-07」，但檔內已有 2026-10-28 的段落；2026-10-28 分流紀錄少列 PR 2817 的 3 個檔案；2026-10-25 分流紀錄沒註明 2782／2784／2786 在上游回 404 | `REVIEW.md` 開頭日期與最新段落一致；兩份 `docs/records/upstream-triage-*.md` 補上述資訊；`python tools/check_links.py` 與 `dev_check.ps1` 綠 | Claude Haiku | `REVIEW.md:3,135`；`docs/records/upstream-triage-2026-10-28.md`；審查員建議見 深掃報告（2026-10-28） |

## 中期：等條件成立再做

| 項目 | 觸發條件 | 驗收方式 | 建議執行者 | 證據 |
| --- | --- | --- | --- | --- |
| 同步上游新 commit 並分流新 PR／issue | `Upstream check` workflow（週一 03:00 UTC）轉 failure，或上游 PR 2809／2817 合併；failure 代表有未審項目，不是工具壞 | 照 `docs/UPSTREAM.md` 步驟：三方合併前掃描隱藏內容（長空白、超長行、`eval`、base64、零寬／雙向字元）；分流寫進新的 `docs/records/upstream-triage-<日期>.md`；動到 C 時先有失敗測試，並在 MSYS2 CLANG64 跑受影響 suite（含 `test_stack_overflow`、`test_watcher`）；`dev_check.ps1` 綠、`check_upstream_updates.py --strict` exit 0、水位推進；提交訊息不寫 `#編號`、需 DCO 簽署 | Claude Sonnet | `docs/UPSTREAM.md`；`docs/DECISIONS.md:12,14`；`docs/records/upstream-triage-2026-10-28.md` 末段觸發條件 |
| 鏡像上游新 release 並替換舊 tag／release | 上游發出比 v0.12.1 更新的 release | release 資產鏡像自上游且雜湊核對相符；tag 指向本 fork 同步到該版本的提交；舊 tag 與 release 移除，遠端 tag 只剩最新一個 | Claude Sonnet（刪遠端 tag 前維護者確認） | `docs/DECISIONS.md:4`；`gh release list -R DeusData/codebase-memory-mcp -L 3` 查詢（上游最近 release） |
| ruff 延後紀錄重新評估 | PyPI 出現比 0.17.3 更新的 ruff，`Dependency freshness` 轉紅 | 用新版跑 gate 規則（`--select E9,F`）通過後更新 `.github/dependency-deferrals.json` 的 `deferredLatest` 與理由，下限不變；`Dependency freshness` success | Claude Haiku | `ddabaffc`；`REVIEW.md:135-` 2026-10-28 段 |
| 在本機驗證 graph-ui 的測試 | 需要在本機改動並驗證 `graph-ui/` | 改用 CI 使用的 Node 版本跑 vitest，結果與 CI 一致；本機 Node 26 目前 22 個檔失敗，修改前基準同樣失敗，與 dompurify 無關 | Claude Sonnet | 接續開發報告（2026-10-2x）「刻意不做／延後」第三條 |

## 刻意不做

| 不做什麼 | 理由 | 什麼情況下重新考慮 |
| --- | --- | --- |
| 刪除非 Windows 的程式碼與文件 | 本專案要能索引各平台、各語言的專案，發佈、測試與 CI 相互引用這些檔案；「Windows 為主」由文件、CI 與 gate 表達（`docs/DECISIONS.md:5`） | 維護者決定縮小產品支援範圍 |
| 重新啟用上游的排程與發佈 workflow（`nightly-soak`、`cache-warm`、`scorecard`、`stale`、`pages`、`release` 等） | 刻意停用，可用 `gh workflow enable` 復原（`docs/DECISIONS.md:11`） | 本 fork 需要自己的夜間 soak 或發佈流程；`docs/UPSTREAM.md` 第 6 步檢查「暫停」的 workflow 上游是否已修好 |
| 在上游 issue 或 PR 留言、開 PR，或在提交訊息寫 `#編號` | 上游 issue／PR 只在本 repo 分流；`#編號` 會連到上游並留下反向連結（`docs/DECISIONS.md:12,14`）；回貢需維護者當次明確同意（`FORK.md:18`） | 維護者在當次對話明確同意回貢某個修正 |
| 修 YAML 過期鎖回收的競態；修 vendored tree-sitter C++ 解析器處理 8000 層 `std::vector<` 的 46 秒 | 前者需崩潰殘留的鎖加兩個微秒內並行的安裝程式，無法重現；後者在 vendored 程式碼內，不在本 fork 修改範圍（`REVIEW.md:148,153`） | 前者出現可重現的案例；後者上游更新 vendored 解析器 |

## 維護方式

fork：只有 `main` 一條分支，直接推送（`FORK.md:14`）。上游每週一由 `Upstream check` 檢查，紅燈走上面的同步列；依賴新鮮度每月 1 日檢查；動 C 原始碼一律先寫會失敗的測試再修（`docs/DECISIONS.md:6`）。

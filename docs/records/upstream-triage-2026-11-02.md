# 上游分流（2026-11-02）

範圍：上游 `main` 由 `83109dd` 前進到 `a5b248f4`（507 個 commit，其中 156 個非 merge；多數是 8 月至 10 月間先在功能分支上完成、10-28 之後才併入 main 的 PR，所以日期早於基準），加上 PR 28 筆（2819–2887）與 issue 27 筆（2818–2894）。淨差異 191 檔，+27550／−6257，其中 `vendored/mimalloc/` 約 23%（升到 3.5.3）。上游沒有發新 release（`git describe` 為 `pkg/go/v0.12.1-933-ga5b248f4`）。本 fork 不在上游留言或開 PR。

## 決定：本輪不合併，整批同步排進 ROADMAP

| 理由 | 證據 |
|---|---|
| 規模超過單輪 | 191 檔、+27.5k／−6.3k；上一輪 42 個 commit、35 檔就需要三方合併加 12 個 suite 的 ASan＋UBSan 驗證 |
| 動到主流程與儲存格式 | `src/pipeline/`、`src/daemon/`、`src/store/` 的多處修正；`24bd38c4` 把 graph digest stream 升到 version 2、`001fbe26` 把 ADR 改存每專案 sidecar DB（`2acbd3b0` 為其後續），屬資料格式變動，需另做相容性驗證 |
| 換 vendored 依賴 | `f9f44b58`／`ddc986ea` 把 mimalloc 升到 3.5.3 並加 `free(NULL)` 預初始化修正；`3d65c609` 定義 `MI_MINGW_UCRT64`。fork 的主環境是 Windows，這批要在 MinGW 實機驗證 |
| 與 fork 分岔重疊 | 動到 `cli.c`、`mcp.c`、`sqlite_writer.c`、`extract_*.c` 等 DIVERGENCE 已登記的檔，需逐檔解衝突 |

不採用單筆 cherry-pick：這批 commit 互相依賴（例如 Go init QN 的 `adff62e7`→`4a78fe6a`→`6e784c38`，Java record accessor 的 `f4c89711`→`729ce339`→`6c52e826`→`679d1e1a`），挑單筆會留下不一致的狀態。觸發條件：維護者排定同步時一次整批三方合併（base `83109dd`）。commit 水位（`reviewed_through`）本輪不推進：`tools/check_divergence.py` 把它當作「fork 已含上游到此為止」的 base，不合併就推進會讓 245 個上游檔被判為未登記的分岔（dev_check 紅燈）。PR 與 issue 水位獨立，已推進到 2887／2894。

## 依主題的處理方向（合併時取用，不另挑）

| 主題 | 代表 commit | 方向 |
|---|---|---|
| Windows 路徑與環境 | `56de8453` 非 ASCII 代理路徑、`25546312`／`26e2d0cb` UTF-8 環境值、`c21c42b4` | 優先取用；對本 fork 主環境最直接 |
| mimalloc 3.5.3、MinGW 測試配置器 | `f9f44b58`、`3d65c609`、`192fdb91`（PR 2850）、`ebdf9baf` | 取用；先確認 `mem_override_win.c` 衝突，並回答 issue 2865 |
| Go 抽取與解析 | `dbba7573`、`adff62e7`、`651ae3c2`、`4d97d781`、`ad789939`、`6e784c38`、`4a78fe6a`、`4b03f7af` | 取用；QN 格式改變，需重建索引的相容性檢查 |
| Java／C#／Elixir／Solidity／Python／TS | `f4c89711` 起的 Java record 系列、`c0a8ed8b` 起的 cs-lsp 系列、`630f8515` 起的 Elixir 系列、`6b56d78e`、`9f5b922c`、`fd9f1d83`、`2a9796e0` | 取用 |
| 管線與 daemon | `2c71746f` 系列未解析呼叫涵蓋率、`db74648c`、`5ca234c2`、`74e301d4`、`2443e92b`、`1e92269d` | 取用；`5ca234c2` 動 watcher 生命週期，合併後要跑 `test_watcher` 與 `test_stack_overflow` |
| 儲存 | `24bd38c4`、`001fbe26`、`8ca83936`、`2479724a` | 取用前先確認舊索引庫升級路徑 |
| MCP 工具 | `b95b4033`／`8af68331`／`19374f2d` BM25、`5a666f2f` file_pattern、`ab8f68a1` trace_path 遞迴、`1c1d5422` | 取用 |
| CLI／安裝／agent 設定 | `55844c9d`、`b94a7d7a`、`53d8bb40` 系列 Codex、`3f612f6b`／`0fd81806`、`98a66b73` 系列 OpenCode、`86320afa` Pi、`40261755` Claude hook 提示 | 取用；Codex、OpenCode、Pi 本 fork 不用，但與安裝程式同檔，整批取用避免分岔（同 10-28 對 Copilot hook 的處理） |
| 外掛市集 | `4c09f7c7` | 取用前確認不新增對外發佈面 |
| CI／依賴 | `9bb891b6`、`339cd516`、`a545a0aa`、`e3a9fa34`、`caad9691`、`54e07958`、`1f56c7d6`、`083bc94d`、`9901feef` | fork 停用了多數上游排程 workflow（`DECISIONS.md`），合併時只取仍啟用的 workflow 的改動 |
| 文件 | `a933d531`、`a36b2ee6`、`42474dd7`、`28d62001` | 上游 `README.md` 改動套到 `README.en.md`，產品事實併入繁中 `README.md` |

合併前須對 `83109dd..a5b248f4` 的新增行做隱藏內容掃描（長空白、超過 400 字元單行、`eval`／`new Function(`、長 base64／hex、零寬與雙向字元）；本輪沒有合併，未掃描。

## PR（2819–2887）

| # | 標題（縮） | 狀態 | 決定 |
|---:|---|---|---|
| 2819 | BM25 候選全部排序 | MERGED | 隨整批同步取用（`b95b4033`） |
| 2822 | 外部管理時用受管二進位路徑 | MERGED | 同上（`55844c9d`） |
| 2823 | search_code 掃描逾時 | CLOSED 未合併（+6750） | 不採；觸發：上游以其他形式重提 |
| 2826 | C 系 `.inc` 選 C++ | MERGED | 同上（`51fd445f`） |
| 2827 | PlantUML 空運算元 id | MERGED | 同上（`d68cd2e2`） |
| 2829、2832 | 同一個 PlantUML 修正的另兩版 | CLOSED 未合併 | 不採；2827 已涵蓋 |
| 2833 | 符號連結的 agent 設定目錄 | MERGED | 同上（`b94a7d7a`） |
| 2838 | 索引 worker 低於正常 CPU 優先權 | OPEN（head `616ba411`） | 條件式：合併後隨同步進來；觸發：上游合併或關閉 |
| 2839 | subagent 無 MCP 時的 CLI 提示 | MERGED | 同上 |
| 2840 | watcher 依上次成本調整節奏 | OPEN（head `5c54201e`） | 條件式；動 watcher，不提前移植 |
| 2841 | search_code 目錄 file_pattern | MERGED | 同上 |
| 2842 | trace_path 遞迴中繼資料 | MERGED | 同上；對應 10-28 分流的 issue 2816 |
| 2849 | macOS runner 升到 macos-15 | MERGED | 只取仍啟用的 workflow |
| 2850 | MinGW 測試配置器用 FLS 初始化 | MERGED | 同上整批取用，需 Windows 實機驗證 |
| 2854 | Angular／動詞後綴 HTTP 方法推斷 | MERGED | 同上 |
| 2861 | vite 6.4.5 | MERGED | 同上；fork 的 Dependabot 另行處理 |
| 2862 | 單獨的 `init` 不綁到 package init | MERGED | 同上 |
| 2864 | ADR sidecar 後續 | MERGED | 同上；先看儲存升級路徑 |
| 2867 | cs-lsp 巢狀型別 | MERGED | 同上 |
| 2872 | apt 鏡像停滯時快速失敗 | MERGED | 只取仍啟用的 workflow |
| 2878、2879 | actions 版本升級 | OPEN（head `df92904b`、`c04e322d`） | 不採；fork 的依賴新鮮度另行管理 |
| 2880 | scorecard-action 2.5.0 | MERGED | 不採；`scorecard` workflow 在 fork 停用 |
| 2885 | 新增 Visual Basic .NET（+1,882,429） | OPEN（head `7d6982c4`） | 條件式：新增整個文法；觸發：上游合併後評估是否取用 |
| 2886 | 嚴格共用跨檔解析器（247 檔、+145,912／−15,056） | OPEN（head `d26e6463`） | 條件式：會改寫解析核心；觸發：上游合併，且需整個重新評估 |
| 2887 | 標準 Linux 二進位改用固定 Ubuntu 22.04 容器 | OPEN（head `0fc2d992`） | 不採；發佈 workflow 在 fork 停用 |

## Issue（2818–2894）

| # | 標題（縮） | 狀態 | 決定 |
|---:|---|---|---|
| 2818、2825、2836、2837 | PlantUML 崩潰、`.inc` 分類、file_pattern、subagent 提示 | CLOSED（已修） | 隨對應 PR 同步取用 |
| 2821 | OpenCode `/cd` 後不跟隨 | CLOSED（not planned） | 不採 |
| 2834、2835 | CLI 每次雜湊整個執行檔；agent 建檔觸發重複重建 | OPEN | 不採（尚無修正）；2835 對應 PR 2840；觸發：上游出現修正 |
| 2844 | autoWatch 回 `not_session_project` | OPEN | 不採；使用問題，尚無修正 |
| 2846 | 更新 vendored tree-sitter-perl | OPEN | 不採；觸發：上游合併 |
| 2848 | 序列管線的圖比平行路徑差 | OPEN | 不採；`db74648c` 已讓背景重整留在平行路徑；觸發：上游出現完整修正 |
| 2856、2873 | 受管路徑跨升級的所有權；Codex 子表安裝失敗 | OPEN | 不採；本 fork 不用 Codex 安裝流程 |
| 2858、2859、2860 | hook-augment 前綴、log 過濾誤判、效能計數 | OPEN | 不採；本 fork 不用此 hook |
| 2865 | Windows 發佈的二進位是否踩到 mimalloc 3.5.3 MinGW loader-hook 崩潰 | OPEN | 與本 fork 主環境直接相關；排進 ROADMAP 同步列，合併時在 Windows 實機驗證；觸發：整批同步 |
| 2869 | TS/JS 未解析的裸 import 依名稱綁錯符號 | OPEN | 不採；尚無修正 |
| 2874、2876、2892、2893、2894 | C# 的 INHERITS 遺失、抽取與解析缺口、CALLS 誤報、介面呼叫、讀寫方向與 ORM | OPEN | 不採；2867 已修一部分，其餘待上游修正 |
| 2877 | Go 套件包裝的二進位驗證取決於期限 | OPEN | 不採；發佈流程，fork 停用 |
| 2881、2882 | MFC 匯出巨集；Kotlin 解析缺口 | OPEN | 不採；尚無修正 |
| 2883、2884 | HANDLES 邊被呼叫包裹丟失；Go 標準庫檔案仍綁同名專案函式 | OPEN | 不採；2884 與 `651ae3c2` 為同一系列的後續，待上游 |

觸發條件：維護者排定整批同步，或上游發出比 v0.12.1 更新的 release；PR 2838、2840、2885、2886 合併時重新評估。

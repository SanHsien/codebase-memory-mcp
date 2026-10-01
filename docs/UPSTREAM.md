# 上游同步

上游：[`DeusData/codebase-memory-mcp`](https://github.com/DeusData/codebase-memory-mcp)（`main`），只追蹤、不推送。

1. `git fetch upstream main`
2. `python tools/check_upstream_updates.py --strict`：列出未審查的 commit、PR、issue（各有獨立基準線，`tools/upstream_baseline.json`）。
3. 逐筆判斷後 merge 或 cherry-pick。
4. `pwsh -NoProfile -File tools\dev_check.ps1`
5. 記入 [`DECISIONS.md`](DECISIONS.md)，驗證後更新 `tools/upstream_baseline.json`。

上游改 `README.md`：整份貼進 `README.en.md`（第一行換回語言切換列），再把產品事實的變動併入繁中 `README.md`。

每週 `upstream-check` workflow 會自動跑第 2 步。

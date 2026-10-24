# 更新紀錄

English: [CHANGELOG.en.md](CHANGELOG.en.md)

只記本 fork 的維護變更；產品變更見上游 release。

## [Unreleased]

- 繁中 `README.md`、維護文件、Windows 維護 gate、上游追蹤與分岔登記檢查、對應 CI。
- 只保留 `main` 分支與最新的上游版本 tag／release（目前 `v0.12.1`，資產鏡像自上游）。
- 上游 open issue／PR 分流紀錄。
- 修正 Windows 相關缺陷並補回歸測試（cypher 溢位與資源耗盡、watcher 中文路徑與磁碟機拔除、`cbm_setenv`、`cbm_readdir`、HTTP 介面、like hints、安裝腳本驗證等），詳見 `REVIEW.md`。
- 第二輪審查修正：store、路由資料流、抽取器、TOML 跨行陣列、檔案大小上限、配置檢查等，詳見 `REVIEW.md`。
- 同步上游到 `e71f23e`（2026-10-07），新增 PR／issue 的增量分流紀錄。
- 第三輪審查（抽取器與全部語言 LSP）：58 項實測重現的卡死、崩潰與錯誤結果修正，詳見 `REVIEW.md`。
- 同步上游到 `ec6427c`（v0.12.0，2026-10-16），新增 PR／issue 的增量分流紀錄；恢復 `test-impact-artifact` workflow。
- 修正 Windows：repo 內名為 `nul` 的檔案讓每次索引都中止（上游 issue #2646）。
- 同步上游到 `a7158ac`（v0.12.1）與 `10ab613`（2026-10-24，共 294 個提交），新增 PR／issue 的增量分流紀錄；watcher UTF-8 路徑與 Rust 泛型 impl 改用上游版本；TS LSP 走訪上限減半（512 層在 ASan 下用完 8 MiB 堆疊）。
- 刪除 `GEMINI.md`，內容併入 `AGENTS.md`。

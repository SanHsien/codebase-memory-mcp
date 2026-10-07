# 更新紀錄

English: [CHANGELOG.en.md](CHANGELOG.en.md)

只記本 fork 的維護變更；產品變更見上游 release。

## [Unreleased]

- 繁中 `README.md`、維護文件、Windows 維護 gate、上游追蹤與分岔登記檢查、對應 CI。
- 只保留 `main` 分支與 `v0.11.0` tag／release。
- 上游 open issue／PR 分流紀錄。
- 修正 Windows 相關缺陷並補回歸測試（cypher 溢位與資源耗盡、watcher 中文路徑與磁碟機拔除、`cbm_setenv`、`cbm_readdir`、HTTP 介面、like hints、安裝腳本驗證等），詳見 `REVIEW.md`。
- 第二輪審查修正：store、路由資料流、抽取器、TOML 跨行陣列、檔案大小上限、配置檢查等，詳見 `REVIEW.md`。
- 同步上游到 `e71f23e`（2026-10-07），新增 PR／issue 的增量分流紀錄。

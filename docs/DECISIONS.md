# 決策紀錄

- **上游基準**：`DeusData/codebase-memory-mcp` `main`（MIT）。同步規則見 [`UPSTREAM.md`](UPSTREAM.md)。
- **只留 `main` 分支、`v0.11.0` tag 與其 release**：上游的其他分支與舊 tag 不留在 fork；名稱與 SHA 存於 [`records/`](records/)，上游 repo 仍保有。
- **不刪非 Windows 的程式碼與文件**：本專案要能索引各平台、各語言的專案，且發佈、測試與 CI 相互引用這些檔案。「Windows 為主」由文件、CI 與 gate 表達。
- **動 C 原始碼的規則**：先寫會失敗的測試、確認失敗再修；本機以 MSYS2 CLANG64 建置與跑相關 suite。
- **README**：上游英文原文移到 `README.en.md`，另寫繁中 `README.md`，避免每次同步整檔衝突。
- **換行**：不加全域 `eol=lf`，上游有依賴 CRLF 的測試語料。
- **不啟用 `scripts/hooks`**：pre-commit 需要 clang／cppcheck／make。
- **commit 加 `-s`**：上游 DCO workflow 要求 `Signed-off-by`。
- **停用上游的排程與發佈 workflow**：`nightly-soak`、`cache-warm`、`scorecard`、`stale`、`pages`、`release`、`issue-labeler`、`label-actions`、`pr-acknowledgement`；可用 `gh workflow enable` 復原。
- **上游 issue／PR 只在本 repo 分流**，不在上游留言或開 PR：見 [`records/upstream-triage-2026-09-29.md`](records/upstream-triage-2026-09-29.md)。

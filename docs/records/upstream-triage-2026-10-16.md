# 上游 issue／PR 分流（2026-10-16，增量）

範圍：上次基準（PR #2557、issue #2558）之後新增的 PR 66 筆、issue 33 筆。程式碼同步到 `ec6427c`（707 個提交，上游 v0.12.0 之後）。

已合併的 PR（43 筆）都已隨本次同步進入本 fork。其餘依標題分類，沒有逐筆重現；與本 fork 修正重疊的 PR 另有註記，上游合併後依 [`DIVERGENCE.md`](../DIVERGENCE.md) 的規則採用上游版本。本 fork 不在上游留言或開 PR。

同步時的衝突處理：

- 上游自己修好、改用上游版本：作用域名稱雜湊索引（`scope.c`／`scope.h`，上游 #1527）、呼叫歸屬的「呼叫×定義」迴圈（`cbm.c`）、Cypher AND/OR/XOR 長鏈與 UNION 長鏈（改為鏈結串列與迴圈，fork 的長度上限與兩個對應測試移除）、`/api/index` 回應跳脫、scoop 版本規則、`source-map-js` 1.2.2、watcher 簽章的 mtime（上游改用 100ns 精度）。
- 合併兩邊：`store.c` 套件查詢保留 fork 的 `ESCAPE` 並採用上游的 `GROUP BY +n.name`；`c_lsp.c` 保留 fork 的深度包裝並加上上游的 `eval_failure` 早退；`py_lsp.c` 保留 fork 的 lambda 重入保護並加上上游的類別主體條件；`php_lsp.c` 採用上游迴圈版 callee 解析。
- `cypher.c`：保留 fork 的 `expr_binary` 配置檢查，並讓上游新的鏈結程式在配置失敗時停止（否則解參考 NULL）。
- 上游的 `test-impact-artifact` 在 `ec6427c` 已轉綠，本 fork 恢復該 workflow（見 [`DECISIONS.md`](../DECISIONS.md)）。

## PR

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2658](https://github.com/DeusData/codebase-memory-mcp/pull/2658) | fix(cli): allow Windows project-lock preparation to complete | OPEN | Windows：處理 issue #2656，隨上游合併後同步 |
| [2654](https://github.com/DeusData/codebase-memory-mcp/pull/2654) | fix(docker): never invent a registry host for an image | OPEN | 隨上游合併後同步 |
| [2653](https://github.com/DeusData/codebase-memory-mcp/pull/2653) | docs(readme): point the main CI badge at the release workflow | OPEN | 隨上游合併後同步 |
| [2652](https://github.com/DeusData/codebase-memory-mcp/pull/2652) | chore(pkg): sync packaging metadata to v0.12.0 | OPEN | 隨上游合併後同步 |
| [2651](https://github.com/DeusData/codebase-memory-mcp/pull/2651) | ci(security): pin the ScanCode install; pluggy 1.7.0 breaks it | MERGED | 已含於本次同步（`ec6427c`） |
| [2650](https://github.com/DeusData/codebase-memory-mcp/pull/2650) | fix(daemon): settings startup survives busy and interrupted writes | MERGED | 已含於本次同步（`ec6427c`） |
| [2648](https://github.com/DeusData/codebase-memory-mcp/pull/2648) | fix(security): allow the bare wget token the shell pass compares | MERGED | 已含於本次同步（`ec6427c`） |
| [2644](https://github.com/DeusData/codebase-memory-mcp/pull/2644) | feat(infra): chart identity by publisher URL, Ansible kube DEPLOYS, exact TOML/INI keys | MERGED | 已含於本次同步（`ec6427c`） |
| [2643](https://github.com/DeusData/codebase-memory-mcp/pull/2643) | feat(infra): env and service joins, kustomize variants, Terraform / Terragrunt, Ansible | MERGED | 已含於本次同步（`ec6427c`） |
| [2642](https://github.com/DeusData/codebase-memory-mcp/pull/2642) | perf(mcp): fast query tools on every platform | MERGED | 已含於本次同步（`ec6427c`） |
| [2641](https://github.com/DeusData/codebase-memory-mcp/pull/2641) | fix(cli): leave a user-owned OpenCode module untouched instead of appending the block | OPEN | 隨上游合併後同步 |
| [2639](https://github.com/DeusData/codebase-memory-mcp/pull/2639) | fix(watcher): handle UTF-8 repository and cache paths on Windows | OPEN | Windows：與本 fork 的 `watcher_stat`（寬字元路徑）重疊；上游合併後比較兩者，採用上游版本並刪 DIVERGENCE 對應說明 |
| [2638](https://github.com/DeusData/codebase-memory-mcp/pull/2638) | fix(soak): the per-call read follows the soak's own latency ceiling | MERGED | 已含於本次同步（`ec6427c`） |
| [2637](https://github.com/DeusData/codebase-memory-mcp/pull/2637) | fix: reconcile spawner-deletion test with the namespace contract; guard empty qsort vectors | CLOSED | 上游已關閉，不處理 |
| [2636](https://github.com/DeusData/codebase-memory-mcp/pull/2636) | docs: update the README for CBM Lens Beta (#2383) | MERGED | 已含於本次同步（`ec6427c`） |
| [2635](https://github.com/DeusData/codebase-memory-mcp/pull/2635) | chore(ui): drop sharp and its LGPL libvips; the license gate allows no exception | MERGED | 已含於本次同步（`ec6427c`） |
| [2633](https://github.com/DeusData/codebase-memory-mcp/pull/2633) | fix(cli): never wait on a stdin stream for a call complete without arguments | MERGED | 已含於本次同步（`ec6427c`） |
| [2632](https://github.com/DeusData/codebase-memory-mcp/pull/2632) | chore(ui): resolve the 17 open Dependabot alerts in graph-ui | MERGED | 已含於本次同步（`ec6427c`） |
| [2631](https://github.com/DeusData/codebase-memory-mcp/pull/2631) | ci(lint): raise the lint job timeout from 30 to 45 minutes | MERGED | 已含於本次同步（`ec6427c`） |
| [2630](https://github.com/DeusData/codebase-memory-mcp/pull/2630) | fix(graph): platform variants keep their path QNs and are related by variant_key | MERGED | 已含於本次同步（`ec6427c`） |
| [2629](https://github.com/DeusData/codebase-memory-mcp/pull/2629) | fix(extract): one definition per site when the raw and preprocessed parses disagree | MERGED | 已含於本次同步（`ec6427c`） |
| [2628](https://github.com/DeusData/codebase-memory-mcp/pull/2628) | feat(index): the documentation pass follows the index mode by default | MERGED | 已含於本次同步（`ec6427c`） |
| [2627](https://github.com/DeusData/codebase-memory-mcp/pull/2627) | fix(doclinks): never qsort an empty rst token vector | MERGED | 已含於本次同步（`ec6427c`） |
| [2626](https://github.com/DeusData/codebase-memory-mcp/pull/2626) | fix(cpp): one spelling per template specialization in the preprocessed pass | MERGED | 已含於本次同步（`ec6427c`） |
| [2625](https://github.com/DeusData/codebase-memory-mcp/pull/2625) | fix(resolve): a bare C call never weighs struct members or methods | MERGED | 已含於本次同步（`ec6427c`） |
| [2624](https://github.com/DeusData/codebase-memory-mcp/pull/2624) | fix(pipeline): keep both nodes when a file and a directory share a stem | MERGED | 已含於本次同步（`ec6427c`） |
| [2623](https://github.com/DeusData/codebase-memory-mcp/pull/2623) | fix(ts): bound recursive member and method lookup | OPEN | 與本 fork `ts_lsp.c` 的深度上限可能重疊；上游合併後比較 |
| [2622](https://github.com/DeusData/codebase-memory-mcp/pull/2622) | fix(store): use 64-bit file offsets in the SQLite page writer | OPEN | `long` 在 Windows 是 32 位元，大於 2 GB 的資料庫會受影響；隨上游合併後同步 |
| [2621](https://github.com/DeusData/codebase-memory-mcp/pull/2621) | perf(extraction): compute a call's URL context once, and only when read | MERGED | 已含於本次同步（`ec6427c`） |
| [2619](https://github.com/DeusData/codebase-memory-mcp/pull/2619) | fix(lsp): no pointer offset on an empty C# or Java definition array | MERGED | 已含於本次同步（`ec6427c`） |
| [2617](https://github.com/DeusData/codebase-memory-mcp/pull/2617) | fix: never qsort an empty k8s vector; split the spawner deletion test | MERGED | 已含於本次同步（`ec6427c`） |
| [2616](https://github.com/DeusData/codebase-memory-mcp/pull/2616) | Merge pull request #1 from DeusData/main | OPEN | 誤開的 PR，不處理 |
| [2615](https://github.com/DeusData/codebase-memory-mcp/pull/2615) | ci(pages): skip deployment in unconfigured forks | OPEN | fork 未啟用 Pages（workflow 已手動停用），隨上游合併後同步 |
| [2613](https://github.com/DeusData/codebase-memory-mcp/pull/2613) | fix(markdown): qualify a repeated heading's Section by its parent headings (#2586) | OPEN | 隨上游合併後同步 |
| [2612](https://github.com/DeusData/codebase-memory-mcp/pull/2612) | test(complexity): pin the Dockerfile fixtures' base image by digest | MERGED | 已含於本次同步（`ec6427c`） |
| [2611](https://github.com/DeusData/codebase-memory-mcp/pull/2611) | fix(doc-links): compare a PDF hit's nodes through same_definition | MERGED | 已含於本次同步（`ec6427c`） |
| [2610](https://github.com/DeusData/codebase-memory-mcp/pull/2610) | feat(index): doc_candidates switch for the doc-code candidate scoring | MERGED | 已含於本次同步（`ec6427c`） |
| [2609](https://github.com/DeusData/codebase-memory-mcp/pull/2609) | test(doc_mentions_go): name the setup step a fixture index is stuck in | MERGED | 已含於本次同步（`ec6427c`） |
| [2608](https://github.com/DeusData/codebase-memory-mcp/pull/2608) | fix(discover): a .gitignore never hides a file git tracks | MERGED | 已含於本次同步（`ec6427c`） |
| [2607](https://github.com/DeusData/codebase-memory-mcp/pull/2607) | fix(graph): the id counter is never lowered after a parallel phase | MERGED | 已含於本次同步（`ec6427c`） |
| [2603](https://github.com/DeusData/codebase-memory-mcp/pull/2603) | fix: the 2026-10-12 dry-run reds (GCC 11 sha256, fuzz link, cypher leak, memwaste budget) | MERGED | 已含於本次同步（`ec6427c`） |
| [2602](https://github.com/DeusData/codebase-memory-mcp/pull/2602) | fix(doc-links): report references into a moved directory as missing (#2588) | OPEN | 隨上游合併後同步 |
| [2601](https://github.com/DeusData/codebase-memory-mcp/pull/2601) | fix(doc-links): report a path::member reference to a missing member (#2587) | OPEN | 隨上游合併後同步 |
| [2600](https://github.com/DeusData/codebase-memory-mcp/pull/2600) | fix(mcp): verify content for freshness and snippets; expose store generation (#2585) | OPEN | 隨上游合併後同步 |
| [2599](https://github.com/DeusData/codebase-memory-mcp/pull/2599) | fix(daemon): wait out Windows local teardown contention | OPEN | Windows：daemon 拆除時的競爭（issue #2598），隨上游合併後同步 |
| [2597](https://github.com/DeusData/codebase-memory-mcp/pull/2597) | Infrastructure mono-repos, end to end — Phases 0–4 | MERGED | 已含於本次同步（`ec6427c`） |
| [2596](https://github.com/DeusData/codebase-memory-mcp/pull/2596) | fix(msan): five "stack-overflow" suites were hiding two uninitialized reads | MERGED | 已含於本次同步（`ec6427c`） |
| [2595](https://github.com/DeusData/codebase-memory-mcp/pull/2595) | fix(pipeline): resolve parent-relative Astro and CSS imports | OPEN | 隨上游合併後同步 |
| [2594](https://github.com/DeusData/codebase-memory-mcp/pull/2594) | fix(parser): keep valid TypeScript import arrays complete | OPEN | 隨上游合併後同步 |
| [2593](https://github.com/DeusData/codebase-memory-mcp/pull/2593) | feat(docs): doc-comment links for eight more languages, diagrams and OpenAPI operations in the graph | MERGED | 已含於本次同步（`ec6427c`） |
| [2592](https://github.com/DeusData/codebase-memory-mcp/pull/2592) | test(cli): the activation-maintenance fixture and the drained probe end on definite events | MERGED | 已含於本次同步（`ec6427c`） |
| [2590](https://github.com/DeusData/codebase-memory-mcp/pull/2590) | test(cli): no timeout decides a cli_scope test; finish keeps the environment | MERGED | 已含於本次同步（`ec6427c`） |
| [2589](https://github.com/DeusData/codebase-memory-mcp/pull/2589) | fix(cli): failed updates and the scope fixture name their cause | MERGED | 已含於本次同步（`ec6427c`） |
| [2583](https://github.com/DeusData/codebase-memory-mcp/pull/2583) | fix(rst): a section's text no longer reads an unwritten byte | MERGED | 已含於本次同步（`ec6427c`） |
| [2582](https://github.com/DeusData/codebase-memory-mcp/pull/2582) | perf(semantic): exact, cheaper doc-section candidate scoring | MERGED | 已含於本次同步（`ec6427c`） |
| [2581](https://github.com/DeusData/codebase-memory-mcp/pull/2581) | perf(mem): recommit purged memory on macOS so phys_footprint drops | MERGED | 已含於本次同步（`ec6427c`） |
| [2579](https://github.com/DeusData/codebase-memory-mcp/pull/2579) | fix(ts-lsp): resolve JSX calls from top-level arrow components | OPEN | 隨上游合併後同步 |
| [2578](https://github.com/DeusData/codebase-memory-mcp/pull/2578) | fix(registry): break suffix_match ties by kind, C# stub, then QN order, not depth | MERGED | 已含於本次同步（`ec6427c`） |
| [2575](https://github.com/DeusData/codebase-memory-mcp/pull/2575) | perf(sha256): compress blocks on ARMv8 SHA2 / x86 SHA-NI (#2441) | MERGED | 已含於本次同步（`ec6427c`） |
| [2574](https://github.com/DeusData/codebase-memory-mcp/pull/2574) | Semantic relations with a judged p; doc sections linked to their code by format and position | MERGED | 已含於本次同步（`ec6427c`） |
| [2573](https://github.com/DeusData/codebase-memory-mcp/pull/2573) | ci: build Linux amd64 release on Ubuntu 22.04 | OPEN | 隨上游合併後同步 |
| [2568](https://github.com/DeusData/codebase-memory-mcp/pull/2568) | fix(discover): map jinja and jinja2 in userconfig language table | OPEN | 隨上游合併後同步 |
| [2567](https://github.com/DeusData/codebase-memory-mcp/pull/2567) | ci(test-impact): give the engine full history; publish maps with an incomplete suite | MERGED | 已含於本次同步（`ec6427c`） |
| [2564](https://github.com/DeusData/codebase-memory-mcp/pull/2564) | Feat/721 vb6 grammar r2 | OPEN | 新語言文法，隨上游合併後同步 |
| [2562](https://github.com/DeusData/codebase-memory-mcp/pull/2562) | fix(memory): update mimalloc to fix glibc 2.44 startup crash | OPEN | mimalloc 更新；與 issue #2606 有關，隨上游合併後同步 |
| [2561](https://github.com/DeusData/codebase-memory-mcp/pull/2561) | fix(store): one process-wide section for a query connection's first access | MERGED | 已含於本次同步（`ec6427c`） |

## Issue

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2657](https://github.com/DeusData/codebase-memory-mcp/issues/2657) | v0.12.0 npm postinstall always fails: linux-amd64 binary (316 MB) exceeds the installer's own MAX_ARCHIVE_MEMBER_BYTES limit (256 MB) | OPEN | npm 安裝流程，平台中立，隨上游 |
| [2656](https://github.com/DeusData/codebase-memory-mcp/issues/2656) | Windows: 100 ms CLI project-lock attempts can livelock before source parsing | OPEN | Windows：上游 PR #2658 處理中，隨上游 |
| [2655](https://github.com/DeusData/codebase-memory-mcp/issues/2655) | Windows: graph DBs larger than 2 GiB silently fail daemon watch registration | OPEN | Windows：大於 2 GiB 的資料庫，與 PR #2622（64 位元檔案位移）同類；追蹤，上游處理後同步 |
| [2649](https://github.com/DeusData/codebase-memory-mcp/issues/2649) | index_repository 导致 oom | OPEN | 平台中立，隨上游 |
| [2647](https://github.com/DeusData/codebase-memory-mcp/issues/2647) | Indexing gap | CLOSED | 上游已關閉，不處理 |
| [2646](https://github.com/DeusData/codebase-memory-mcp/issues/2646) | Windows: a file named `nul` in the repo makes every index_repository abort with aborted_previous_preserved (0.11.0) | OPEN | Windows：本機實測重現（名為 `nul` 的檔案讓每次索引都中止），本 fork 已修正並補回歸測試（`pipeline_incremental.c`，見 DIVERGENCE） |
| [2640](https://github.com/DeusData/codebase-memory-mcp/issues/2640) | RFC: Optional local EmbeddingGemma 2 backend for semantic search | OPEN | 平台中立，隨上游 |
| [2634](https://github.com/DeusData/codebase-memory-mcp/issues/2634) | Proposal: Run git, grep and find helpers with argv instead of a shell | OPEN | 平台中立，隨上游 |
| [2620](https://github.com/DeusData/codebase-memory-mcp/issues/2620) | Objective-C headers are parsed as C++, producing wrong nodes | OPEN | 平台中立，隨上游 |
| [2618](https://github.com/DeusData/codebase-memory-mcp/issues/2618) | Opencode V2.x -- cbm-augment.ts fails | OPEN | 平台中立，隨上游 |
| [2614](https://github.com/DeusData/codebase-memory-mcp/issues/2614) | detect_changes: report the code a changed documentation section references | OPEN | 平台中立，隨上游 |
| [2606](https://github.com/DeusData/codebase-memory-mcp/issues/2606) | Windows/MSYS2-gcc15: self-built main hangs at scale; self-built v0.11.0 crashes (mimalloc corrupted free list) — released binary is fine | OPEN | Windows：MSYS2 GCC 15 自建版卡死或崩潰；本 fork 用 CLANG64 建置（見 DEVELOPMENT），追蹤 |
| [2605](https://github.com/DeusData/codebase-memory-mcp/issues/2605) | Rust: field-receiver method calls falling to the registry's name fallback bind an arbitrary same-named method (trait stub wins; Session.push_daily shows 0 callers) | OPEN | 平台中立，隨上游 |
| [2604](https://github.com/DeusData/codebase-memory-mcp/issues/2604) | Added 1-click config generator and setup guide for codebase-memory-mcp on MCP ServerList | OPEN | 推廣文，不處理 |
| [2598](https://github.com/DeusData/codebase-memory-mcp/issues/2598) | Windows local teardown treats live startup contention as a cleanup failure | OPEN | Windows：上游 PR #2599 處理中，隨上游 |
| [2591](https://github.com/DeusData/codebase-memory-mcp/issues/2591) | query_graph: a WHERE comparison between two properties (a.file_path = b.file_path) fails with "expected value at pos N" | OPEN | 平台中立，隨上游 |
| [2588](https://github.com/DeusData/codebase-memory-mcp/issues/2588) | References into a moved directory vanish without unresolved rows | OPEN | 平台中立，隨上游 |
| [2587](https://github.com/DeusData/codebase-memory-mcp/issues/2587) | A `path::member` reference to a missing member silently becomes a file edge | OPEN | 平台中立，隨上游 |
| [2586](https://github.com/DeusData/codebase-memory-mcp/issues/2586) | Repeated Markdown headings collapse into one Section and misattribute references | OPEN | 平台中立，隨上游 |
| [2585](https://github.com/DeusData/codebase-memory-mcp/issues/2585) | Tool results cannot be tied to one coherent, content-verified index snapshot | OPEN | 平台中立，隨上游 |
| [2584](https://github.com/DeusData/codebase-memory-mcp/issues/2584) | Windows: index_repository fails with pipeline.stage lock_failed errno=22 on w64devkit | OPEN | Windows（w64devkit）：本 fork 不支援該工具鏈，追蹤 |
| [2580](https://github.com/DeusData/codebase-memory-mcp/issues/2580) | Allow users to selectively choose which harnesses to integrate with codebase-mcp | OPEN | 平台中立，隨上游 |
| [2577](https://github.com/DeusData/codebase-memory-mcp/issues/2577) | Use bounded streaming compression for artifact export instead of whole-snapshot buffers | OPEN | 平台中立，隨上游 |
| [2576](https://github.com/DeusData/codebase-memory-mcp/issues/2576) | TypeScript: import('…').T[] inside a generic call's type argument marks a valid file parse_partial and drops later declarations | OPEN | 平台中立，隨上游 |
| [2572](https://github.com/DeusData/codebase-memory-mcp/issues/2572) | Indexing gap: sql/wisdom_cs | OPEN | 平台中立，隨上游 |
| [2571](https://github.com/DeusData/codebase-memory-mcp/issues/2571) | Indexing gap: Users-gautamparab-Projects-gp | OPEN | 平台中立，隨上游 |
| [2570](https://github.com/DeusData/codebase-memory-mcp/issues/2570) | Indexing gap: reference | OPEN | 平台中立，隨上游 |
| [2569](https://github.com/DeusData/codebase-memory-mcp/issues/2569) | Daemon startup exceeds hardcoded 30s client-accept timeout on slow devices (Termux proot, aarch64) | OPEN | 慢速裝置（Termux）的 daemon 啟動逾時，平台中立，隨上游 |
| [2566](https://github.com/DeusData/codebase-memory-mcp/issues/2566) | Indexing gap: resources/css | OPEN | 平台中立，隨上游 |
| [2565](https://github.com/DeusData/codebase-memory-mcp/issues/2565) | Support Zcode | OPEN | 功能請求，隨上游 |
| [2563](https://github.com/DeusData/codebase-memory-mcp/issues/2563) | extra_extensions silently ignores jinja2 — Flask/Jinja .html templates cannot use the vendored Jinja2 grammar | OPEN | 平台中立，隨上游 |
| [2560](https://github.com/DeusData/codebase-memory-mcp/issues/2560) | Index worker spins on a large pg_dump .sql with escaped apostrophes | OPEN | 平台中立，隨上游 |
| [2559](https://github.com/DeusData/codebase-memory-mcp/issues/2559) | Regression since 0.10.0: JSX render edges dropped from arrow-function components (const X = () => <Child/>) | OPEN | 平台中立，隨上游 |

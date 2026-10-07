# 上游 issue／PR 分流（2026-10-07，增量）

範圍：上次基準（PR #2473、issue #2460）之後新增的 PR 67 筆、issue 17 筆。程式碼同步到 `e71f23e`（133 個提交）。

已合併的 PR（20 筆）都已隨本次同步進入本 fork。其餘依標題分類，沒有逐筆重現；與本 fork 修正重疊的 PR 另有註記，上游合併後依 [`DIVERGENCE.md`](../DIVERGENCE.md) 的規則採用上游版本。本 fork 不在上游留言或開 PR。

同步時的衝突處理：

- `scripts/setup.sh`、`scripts/setup-windows.ps1`：上游改為委託 `install.sh`／`install.ps1`，兩者都強制驗證 SHA-256，採用上游版本，刪除本 fork 的雜湊修改。
- `src/store/store.c` QN 後綴查詢：上游自己加了跳脫；保留本 fork 的精確尾端比對（LIKE 不分大小寫）。
- `.gitignore` 比對引擎：上游搬到 `gitignore_core.c`，Windows 大小寫修正隨之移植。

## PR

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2557](https://github.com/DeusData/codebase-memory-mcp/pull/2557) | build(deps): Bump source-map-js from 1.2.1 to 1.2.2 in /graph-ui in the npm_and_yarn group across 1 directory | OPEN | Dependabot，隨上游合併 |
| [2556](https://github.com/DeusData/codebase-memory-mcp/pull/2556) | fix(mcp): log the server/discover connect probe as info, not an error | OPEN | 隨上游合併後同步 |
| [2555](https://github.com/DeusData/codebase-memory-mcp/pull/2555) | fix(pipeline): resolve constants in JS/TS template URLs | OPEN | 隨上游合併後同步 |
| [2554](https://github.com/DeusData/codebase-memory-mcp/pull/2554) | ci: bump macOS runners from macos-14 to macos-15 | OPEN | 隨上游合併後同步 |
| [2551](https://github.com/DeusData/codebase-memory-mcp/pull/2551) | feat(doc-links): Markdown, ADR, reST, AsciiDoc and PDF sections link to the code they name | OPEN | 隨上游合併後同步 |
| [2550](https://github.com/DeusData/codebase-memory-mcp/pull/2550) | feat(doc-links): C# doc-comment references become MENTIONS edges | OPEN | 隨上游合併後同步 |
| [2549](https://github.com/DeusData/codebase-memory-mcp/pull/2549) | test: keep ASan stack-use-after-return off outside the diag lane | OPEN | 隨上游合併後同步 |
| [2548](https://github.com/DeusData/codebase-memory-mcp/pull/2548) | ci: analyse and score only the newest main commit | OPEN | 隨上游合併後同步 |
| [2547](https://github.com/DeusData/codebase-memory-mcp/pull/2547) | fix(haskell): stop escaped char literals at the closing quote (#2439) | OPEN | 隨上游合併後同步 |
| [2546](https://github.com/DeusData/codebase-memory-mcp/pull/2546) | Rfc 001 | CLOSED | 上游已關閉，不處理 |
| [2545](https://github.com/DeusData/codebase-memory-mcp/pull/2545) | fix(ui): Atlas fixes from the review call and two hand tests | OPEN | 隨上游合併後同步 |
| [2544](https://github.com/DeusData/codebase-memory-mcp/pull/2544) | fix(build): reconcile three call sites after concurrent merges | MERGED | 已含於本次同步（`e71f23e`） |
| [2543](https://github.com/DeusData/codebase-memory-mcp/pull/2543) | fix(extract): C typedefs become nodes; macros and enumerators get their own QNs | OPEN | 隨上游合併後同步 |
| [2541](https://github.com/DeusData/codebase-memory-mcp/pull/2541) | fix(cpp-lsp): use direct includes to disambiguate receiver targets | OPEN | 隨上游合併後同步 |
| [2535](https://github.com/DeusData/codebase-memory-mcp/pull/2535) | perf(python): narrow class and wildcard registry scans | OPEN | 隨上游合併後同步 |
| [2532](https://github.com/DeusData/codebase-memory-mcp/pull/2532) | perf(python): avoid full registry scans for submodule probes | OPEN | 隨上游合併後同步 |
| [2531](https://github.com/DeusData/codebase-memory-mcp/pull/2531) | fix(python): preserve ordered namespace metadata | OPEN | 隨上游合併後同步 |
| [2529](https://github.com/DeusData/codebase-memory-mcp/pull/2529) | fix(python): resolve class-body and decorator calls | OPEN | 隨上游合併後同步 |
| [2528](https://github.com/DeusData/codebase-memory-mcp/pull/2528) | fix(python): separate package symbol scope from file identity | OPEN | 隨上游合併後同步 |
| [2526](https://github.com/DeusData/codebase-memory-mcp/pull/2526) | fix(mcp): discover projects with underscore-prefixed names | MERGED | 已含於本次同步（`e71f23e`） |
| [2525](https://github.com/DeusData/codebase-memory-mcp/pull/2525) | fix(python): join module-level calls to exact LSP resolutions | OPEN | 隨上游合併後同步 |
| [2524](https://github.com/DeusData/codebase-memory-mcp/pull/2524) | fix(mcp): avoid misleading empty-search filter hints | MERGED | 已含於本次同步（`e71f23e`） |
| [2523](https://github.com/DeusData/codebase-memory-mcp/pull/2523) | fix(typescript): resolve constructor properties and new-initialized fields | MERGED | 已含於本次同步（`e71f23e`） |
| [2522](https://github.com/DeusData/codebase-memory-mcp/pull/2522) | fix(lsp): bound repeated resolver work without truncating large files | OPEN | 隨上游合併後同步 |
| [2521](https://github.com/DeusData/codebase-memory-mcp/pull/2521) | Add new binary files for ChromaDB data storage | OPEN | 無關內容（加入 ChromaDB 二進位檔），不處理 |
| [2520](https://github.com/DeusData/codebase-memory-mcp/pull/2520) | fix(php): keep proven vendor types out of project name fallbacks | OPEN | 隨上游合併後同步 |
| [2519](https://github.com/DeusData/codebase-memory-mcp/pull/2519) | fix: exclude client navigation from inferred HTTP calls | OPEN | 隨上游合併後同步 |
| [2518](https://github.com/DeusData/codebase-memory-mcp/pull/2518) | test(mcp): isolate search scratch cleanup fixtures | MERGED | 已含於本次同步（`e71f23e`） |
| [2517](https://github.com/DeusData/codebase-memory-mcp/pull/2517) | fix(cli): keep project indexes by default during uninstall | MERGED | 已含於本次同步（`e71f23e`） |
| [2516](https://github.com/DeusData/codebase-memory-mcp/pull/2516) | fix(mcp): release path filters on search setup errors | MERGED | 已含於本次同步（`e71f23e`） |
| [2515](https://github.com/DeusData/codebase-memory-mcp/pull/2515) | Prevent PHP vendor imports from binding unrelated project symbols | OPEN | 隨上游合併後同步 |
| [2514](https://github.com/DeusData/codebase-memory-mcp/pull/2514) | Fix explicit overrides through non-redeclaring ancestors | OPEN | 隨上游合併後同步 |
| [2513](https://github.com/DeusData/codebase-memory-mcp/pull/2513) | Preserve topic identities while folding local URL constants | OPEN | 隨上游合併後同步 |
| [2512](https://github.com/DeusData/codebase-memory-mcp/pull/2512) | test(parse): cover the reported TSX ampersand fragment (#2481) | OPEN | 隨上游合併後同步 |
| [2511](https://github.com/DeusData/codebase-memory-mcp/pull/2511) | fix(cypher): count DISTINCT rows against the row cap | MERGED | 已含於本次同步（`e71f23e`） |
| [2510](https://github.com/DeusData/codebase-memory-mcp/pull/2510) | fix(subprocess): close inherited descriptors with kernel range operation | MERGED | 已含於本次同步（`e71f23e`） |
| [2509](https://github.com/DeusData/codebase-memory-mcp/pull/2509) | fix(pipeline): resolve PSR-4 imports to exact class files | MERGED | 已含於本次同步（`e71f23e`） |
| [2508](https://github.com/DeusData/codebase-memory-mcp/pull/2508) | test(mcp): cover project discovery after store switches (#1115) | OPEN | 隨上游合併後同步 |
| [2507](https://github.com/DeusData/codebase-memory-mcp/pull/2507) | build(deps): Bump actions/attest from 4.1.0 to 4.2.2 | OPEN | Dependabot，隨上游合併 |
| [2506](https://github.com/DeusData/codebase-memory-mcp/pull/2506) | build(deps): Bump github/codeql-action/init from 4.38.1 to 4.38.2 | OPEN | Dependabot，隨上游合併 |
| [2505](https://github.com/DeusData/codebase-memory-mcp/pull/2505) | build(deps): Bump github/codeql-action/upload-sarif from 4.38.1 to 4.38.2 | OPEN | Dependabot，隨上游合併 |
| [2504](https://github.com/DeusData/codebase-memory-mcp/pull/2504) | build(deps): Bump github/codeql-action/analyze from 4.38.1 to 4.38.2 | OPEN | Dependabot，隨上游合併 |
| [2503](https://github.com/DeusData/codebase-memory-mcp/pull/2503) | build(deps): Bump msys2/setup-msys2 from 2.32.0 to 2.33.0 | OPEN | Dependabot，隨上游合併 |
| [2502](https://github.com/DeusData/codebase-memory-mcp/pull/2502) | fix(cypher): defer WHERE filtering until referenced variables are available | OPEN | 隨上游合併後同步 |
| [2499](https://github.com/DeusData/codebase-memory-mcp/pull/2499) | fix(ci): parse clang-tidy analyzer warning suffix | OPEN | 隨上游合併後同步 |
| [2498](https://github.com/DeusData/codebase-memory-mcp/pull/2498) | fix(python): retain typed fields across file boundaries | MERGED | 已含於本次同步（`e71f23e`） |
| [2497](https://github.com/DeusData/codebase-memory-mcp/pull/2497) | fix(extract): preserve Go router group prefixes | OPEN | 隨上游合併後同步 |
| [2496](https://github.com/DeusData/codebase-memory-mcp/pull/2496) | fix(mcp): detect changes from the repository default branch | MERGED | 已含於本次同步（`e71f23e`） |
| [2495](https://github.com/DeusData/codebase-memory-mcp/pull/2495) | fix(pipeline): key GraphQL routes by operation name | MERGED | 已含於本次同步（`e71f23e`） |
| [2494](https://github.com/DeusData/codebase-memory-mcp/pull/2494) | fix(index): report clean worker exits without a response | MERGED | 已含於本次同步（`e71f23e`） |
| [2493](https://github.com/DeusData/codebase-memory-mcp/pull/2493) | fix: avoid null arguments for empty project and edge buffers | OPEN | 與本 fork 的配置檢查重疊；上游合併後比對 |
| [2492](https://github.com/DeusData/codebase-memory-mcp/pull/2492) | freebsd support | OPEN | 隨上游合併後同步 |
| [2491](https://github.com/DeusData/codebase-memory-mcp/pull/2491) | fix(mcp): keep search matches within the resolved project root | OPEN | 搜尋範圍邊界；上游合併後隨同步帶入 |
| [2490](https://github.com/DeusData/codebase-memory-mcp/pull/2490) | fix(workspace): classify manifest entries in their resolved form | OPEN | 隨上游合併後同步 |
| [2489](https://github.com/DeusData/codebase-memory-mcp/pull/2489) | fix(workspace): skip links during path-alias discovery | MERGED | 已含於本次同步（`e71f23e`） |
| [2488](https://github.com/DeusData/codebase-memory-mcp/pull/2488) | fix(daemon): preserve capacity rejection before the client HELLO | MERGED | 已含於本次同步（`e71f23e`） |
| [2487](https://github.com/DeusData/codebase-memory-mcp/pull/2487) | ci: preserve sanitizer diagnostics and failed suite logs | OPEN | 隨上游合併後同步 |
| [2485](https://github.com/DeusData/codebase-memory-mcp/pull/2485) | ci: retry MSan apt updates after transient repository errors | OPEN | 隨上游合併後同步 |
| [2484](https://github.com/DeusData/codebase-memory-mcp/pull/2484) | fix(cypher): treat unbound variables as unknown in early WHERE | CLOSED | 上游已關閉，不處理 |
| [2483](https://github.com/DeusData/codebase-memory-mcp/pull/2483) | feat: add agentty agent support to the install CLI | OPEN | 隨上游合併後同步 |
| [2480](https://github.com/DeusData/codebase-memory-mcp/pull/2480) | fix(setup): install through install.sh / install.ps1, one install path | MERGED | 已含於本次同步（`e71f23e`） |
| [2479](https://github.com/DeusData/codebase-memory-mcp/pull/2479) | fix(workspace): compare the home directory in its canonical form | MERGED | 已含於本次同步（`e71f23e`） |
| [2478](https://github.com/DeusData/codebase-memory-mcp/pull/2478) | fix(extract): walk nested syntax iteratively, bound the args formatter | OPEN | 與本 fork 的抽取器迭代化（dbt、內嵌區塊）重疊；上游合併後採用上游版本 |
| [2477](https://github.com/DeusData/codebase-memory-mcp/pull/2477) | fix(cypher): walk operator chains and UNION branches in loops | OPEN | 與本 fork 的 Cypher AND/OR/UNION 上限重疊；上游合併後採用上游版本 |
| [2476](https://github.com/DeusData/codebase-memory-mcp/pull/2476) | fix(ui): index job carries the resolved root; status body sized for it | OPEN | 與本 fork 的 `/api/index` 路徑截斷修正重疊；上游合併後採用上游版本 |
| [2475](https://github.com/DeusData/codebase-memory-mcp/pull/2475) | fix(regex): size a pattern before compiling and refuse oversized ones | MERGED | 已含於本次同步（`e71f23e`） |
| [2474](https://github.com/DeusData/codebase-memory-mcp/pull/2474) | fix(mcp): clamp search_code context on both sides, add sink ceilings | MERGED | 已含於本次同步（`e71f23e`） |

## Issue

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2558](https://github.com/DeusData/codebase-memory-mcp/issues/2558) | IMPORTS: parent-relative ("../") imports of non-JS/TS files (.astro, .css) never resolve — silent empty "who uses" results | OPEN | 平台中立，隨上游 |
| [2553](https://github.com/DeusData/codebase-memory-mcp/issues/2553) | Pi: use pi's native MCP client (mcp.json) instead of the generated cbmem.ts bridge | OPEN | 平台中立，隨上游 |
| [2552](https://github.com/DeusData/codebase-memory-mcp/issues/2552) | Windows code integrating and signed executable | OPEN | Windows 簽章執行檔：屬發佈流程，本 fork 不簽章，追蹤 |
| [2542](https://github.com/DeusData/codebase-memory-mcp/issues/2542) | C++: indirect include plus forward declaration binds one receiver to unrelated duplicate classes | OPEN | 平台中立，隨上游 |
| [2540](https://github.com/DeusData/codebase-memory-mcp/issues/2540) | JS/TS: fetch(`${BASE_URL}/v1/x`) with a base-URL constant produces no HTTP_CALLS (template flattened to "{}/v1/x" and rejected) | OPEN | 平台中立，隨上游 |
| [2539](https://github.com/DeusData/codebase-memory-mcp/issues/2539) | Go/sarama: consumer group ID is recorded as a Kafka topic (false ASYNC_CALLS), and real topics from ProducerMessage{Topic} / ConsumerGroup.Consume are not extracted | OPEN | 平台中立，隨上游 |
| [2538](https://github.com/DeusData/codebase-memory-mcp/issues/2538) | Go: standard-library HTTP client calls (http.Get / http.Post / http.NewRequest) never produce HTTP_CALLS edges | OPEN | 平台中立，隨上游 |
| [2537](https://github.com/DeusData/codebase-memory-mcp/issues/2537) | Go: net/http ServeMux patterns with a method or host ("POST /v1/x", "GET host/x") produce no Route nodes | OPEN | 平台中立，隨上游 |
| [2536](https://github.com/DeusData/codebase-memory-mcp/issues/2536) | Indexing gap: mobile-app/src/components/meeting | OPEN | 平台中立，隨上游 |
| [2534](https://github.com/DeusData/codebase-memory-mcp/issues/2534) | Daemon retries a refused /Users/hirak watch hundreds of times instead of skipping once | OPEN | daemon 重試被拒的 watch：追蹤 |
| [2533](https://github.com/DeusData/codebase-memory-mcp/issues/2533) | Regression in v0.11.0: watcher.changed/strategy=git self-triggers a continuous reindex loop (reopens #1953) | OPEN | watcher 迴圈重新索引：本機 Windows 未重現，追蹤 |
| [2530](https://github.com/DeusData/codebase-memory-mcp/issues/2530) | 0.11.0: stdio MCP initialize takes 19–38s on macOS, exceeding client connect timeouts | OPEN | 平台中立，隨上游 |
| [2527](https://github.com/DeusData/codebase-memory-mcp/issues/2527) | Add codebase-memory-mcp to awesome-ai-plugins? | OPEN | 平台中立，隨上游 |
| [2501](https://github.com/DeusData/codebase-memory-mcp/issues/2501) | Cross-repo pass silently stops matching after 4,096 call edges per project pair | OPEN | 跨 repo 呼叫邊 4096 上限：設計上限，追蹤上游決定 |
| [2486](https://github.com/DeusData/codebase-memory-mcp/issues/2486) | codebase-memory-mcp doesn't run | OPEN | 資訊不足，等回報者 |
| [2482](https://github.com/DeusData/codebase-memory-mcp/issues/2482) | C++ indexing gap: empty braced default argument (`parameter = {}`) | OPEN | 平台中立，隨上游 |
| [2481](https://github.com/DeusData/codebase-memory-mcp/issues/2481) | Indexing gap: frontend/src/components/feedback | OPEN | 平台中立，隨上游 |

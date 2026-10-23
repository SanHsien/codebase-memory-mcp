# 上游 issue／PR 分流（2026-10-24，增量）

範圍：上次基準（PR 2658、issue 2657）之後新增的 PR 75 筆、issue 28 筆。程式碼分兩段同步：先到 `a7158ac`（上游 v0.12.1），再到 `10ab613`（共 294 個提交）。

已合併的 PR（44 筆）都已隨本次同步進入本 fork。其餘依標題分類，沒有逐筆重現；與本 fork 修正重疊的 PR 另有註記，上游合併後依 [`DIVERGENCE.md`](../DIVERGENCE.md) 的規則採用上游版本。本 fork 不在上游留言或開 PR。

同步時的衝突處理：

- 上游自己修好、改用上游版本：Rust 泛型 impl 的 self 型別去掉型別參數（`rust_lsp.c`，上游 `f75af00a`；fork 的回歸測試保留）、watcher 的 UTF-8 路徑（上游 PR 2639，fork 的 `watcher_stat` 移除）。
- 合併兩邊：`php_lsp.c` 類別欄位收集保留 fork 的可成長堆疊並加上上游的 trait 索引；`watcher.c` 以上游的路徑查詢為底，保留 fork 的「磁碟機不存在視為不確定」；`test_language_count_contract.sh` 採用上游清單，`README.md` 換成 `README.en.md`。
- `test_stack_overflow.c`：以上游版本為底，插回 fork 的純新增測試與註冊（510 行），避免 fork 測試落在上游註解中間。
- `README.en.md`：上游原文加回本 fork 的「Multi-agent pipeline」一節。

前次點名的 Windows 項目：issue 2655（大於 2 GiB 的資料庫）已由上游關閉（隨 PR 2639）；PR 2639 已合併並採用；PR 2622（64 位元檔案位移）、PR 2658／issue 2656（專案鎖活鎖）仍 OPEN，繼續追蹤；PR 2599（本機 teardown 爭用）已合併並含於本次同步。

## PR

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2774](https://github.com/DeusData/codebase-memory-mcp/pull/2774) | extract: fold prefixed and triple-quoted Python string URLs | OPEN | 與本 fork `extract_calls.c` 的字串前綴／三引號剝除重疊；上游合併後採用上游版本並刪 DIVERGENCE 對應說明 |
| [2771](https://github.com/DeusData/codebase-memory-mcp/pull/2771) | feat(registry): break a Rust suffix tie with the receiver field's type | OPEN | 隨上游合併後同步 |
| [2770](https://github.com/DeusData/codebase-memory-mcp/pull/2770) | fix(rust): fold struct fields so cross-file field receivers resolve | OPEN | 隨上游合併後同步 |
| [2769](https://github.com/DeusData/codebase-memory-mcp/pull/2769) | fix(go-lsp): follow a type alias declared in another file of the package | OPEN | 隨上游合併後同步 |
| [2766](https://github.com/DeusData/codebase-memory-mcp/pull/2766) | install.ps1: report a checksum-verified layout mismatch separately from an unsafe archive | OPEN | Windows：`install.ps1` 錯誤訊息區分，隨上游合併後同步 |
| [2764](https://github.com/DeusData/codebase-memory-mcp/pull/2764) | fix(cypher): return one row for an ungrouped aggregate over empty input | OPEN | 隨上游合併後同步 |
| [2762](https://github.com/DeusData/codebase-memory-mcp/pull/2762) | build(deps): Bump @vitest/mocker from 4.1.11 to 4.1.12 in /graph-ui in the npm_and_yarn group across 1 directory | OPEN | 隨上游合併後同步 |
| [2761](https://github.com/DeusData/codebase-memory-mcp/pull/2761) | feat(language): map GLSL compute, geometry and tessellation stages | OPEN | 隨上游合併後同步 |
| [2760](https://github.com/DeusData/codebase-memory-mcp/pull/2760) | fix(extract): YAML walkers grow their stacks instead of capping content | OPEN | 與本 fork `extract_unified.c` 的 YAML 可成長堆疊重疊；上游合併後採用上游版本並刪 DIVERGENCE 對應說明 |
| [2757](https://github.com/DeusData/codebase-memory-mcp/pull/2757) | fix: preserve literal input and static asset boundaries | OPEN | 隨上游合併後同步 |
| [2755](https://github.com/DeusData/codebase-memory-mcp/pull/2755) | ci: parallelize cppcheck with isolated whole-program analysis | OPEN | 隨上游合併後同步 |
| [2754](https://github.com/DeusData/codebase-memory-mcp/pull/2754) | fix(pipeline): a bare C reference never binds a struct Field | OPEN | 隨上游合併後同步 |
| [2753](https://github.com/DeusData/codebase-memory-mcp/pull/2753) | fix(ui): hide stale focus warning while arranging graph | MERGED | 已含於本次同步（`10ab613`） |
| [2752](https://github.com/DeusData/codebase-memory-mcp/pull/2752) | fix(routes): read Go 1.22 ServeMux method and host patterns | OPEN | 隨上游合併後同步 |
| [2749](https://github.com/DeusData/codebase-memory-mcp/pull/2749) | refactor(search): run POSIX unscoped scan as argv stages, no shell (#2634) | OPEN | 隨上游合併後同步 |
| [2747](https://github.com/DeusData/codebase-memory-mcp/pull/2747) | feat(extraction): route Next.js App Router handlers (#759) | OPEN | 隨上游合併後同步 |
| [2746](https://github.com/DeusData/codebase-memory-mcp/pull/2746) | fix(ts-lsp): resolve calls through renamed named imports | OPEN | 隨上游合併後同步 |
| [2744](https://github.com/DeusData/codebase-memory-mcp/pull/2744) | fix(cypher): support property-to-property comparisons in WHERE (#2591) | OPEN | Cypher 屬性對屬性比較（issue 2591），隨上游合併後同步 |
| [2741](https://github.com/DeusData/codebase-memory-mcp/pull/2741) | build(deps-dev): Bump vite from 6.4.3 to 6.4.4 in /graph-ui in the npm_and_yarn group across 1 directory | CLOSED | 上游已關閉，不處理 |
| [2740](https://github.com/DeusData/codebase-memory-mcp/pull/2740) | build(deps): Bump certifi from 2026.7.22 to 2026.10.19 in /pkg/pypi in the pip group across 1 directory | CLOSED | 上游已關閉，不處理 |
| [2739](https://github.com/DeusData/codebase-memory-mcp/pull/2739) | fix(deps): patch Vite and certifi security advisories | MERGED | 已含於本次同步（`10ab613`） |
| [2738](https://github.com/DeusData/codebase-memory-mcp/pull/2738) | fix(pipeline): emit and reconstruct route properties as valid JSON | OPEN | 與本 fork `pass_route_nodes.c` 的名稱跳脫重疊；上游合併後採用上游版本並刪 DIVERGENCE 對應說明 |
| [2737](https://github.com/DeusData/codebase-memory-mcp/pull/2737) | fix: watch explicit MCP index targets | OPEN | watcher 監看明確指定的索引目標（issue 2733），隨上游合併後同步 |
| [2735](https://github.com/DeusData/codebase-memory-mcp/pull/2735) | docs: refresh Lens recordings and correct README claims | MERGED | 已含於本次同步（`10ab613`） |
| [2734](https://github.com/DeusData/codebase-memory-mcp/pull/2734) | ci(codeql): CodeQL as PR CI jobs (first-party, per language) + full scan on main, dry run and release | MERGED | 已含於本次同步（`10ab613`） |
| [2732](https://github.com/DeusData/codebase-memory-mcp/pull/2732) | fix(ci): repair language count and Windows CodeQL retrace contracts | MERGED | 已含於本次同步（`10ab613`） |
| [2731](https://github.com/DeusData/codebase-memory-mcp/pull/2731) | fix(mem): vendor mimalloc 3.5.3 so free(NULL) before init is safe (supersedes #2562) | OPEN | mimalloc 3.5.3（issue 2763，Linux glibc 2.44 啟動崩潰），隨上游合併後同步 |
| [2729](https://github.com/DeusData/codebase-memory-mcp/pull/2729) | docs: sharpen MCP positioning and refresh website discovery | MERGED | 已含於本次同步（`10ab613`） |
| [2727](https://github.com/DeusData/codebase-memory-mcp/pull/2727) | ci: bump the CodeQL actions to v4.39.0 together and group their Dependabot updates | OPEN | 隨上游合併後同步 |
| [2726](https://github.com/DeusData/codebase-memory-mcp/pull/2726) | docs: refresh project messaging and CBM Lens website | MERGED | 已含於本次同步（`10ab613`） |
| [2724](https://github.com/DeusData/codebase-memory-mcp/pull/2724) | fix(discover): keep JVM packages named like build dirs (part of #2661) | OPEN | 隨上游合併後同步 |
| [2721](https://github.com/DeusData/codebase-memory-mcp/pull/2721) | docs: propose phased TypeScript reference improvements (#2709) | OPEN | 隨上游合併後同步 |
| [2720](https://github.com/DeusData/codebase-memory-mcp/pull/2720) | fix(discover): a symlink must not abort a bounded count (carries #1803, fixes #2701) | MERGED | 已含於本次同步（`10ab613`） |
| [2719](https://github.com/DeusData/codebase-memory-mcp/pull/2719) | feat(cli): give Codex shell searches graph context through a PreToolUse hook | OPEN | 隨上游合併後同步 |
| [2717](https://github.com/DeusData/codebase-memory-mcp/pull/2717) | docs(troubleshooting): add Windows troubleshooting guide | OPEN | Windows：疑難排解文件，隨上游合併後同步 |
| [2716](https://github.com/DeusData/codebase-memory-mcp/pull/2716) | feat(plugin): Claude Code plugin marketplace, discovery slice (#1188 slice 1) | OPEN | 隨上游合併後同步 |
| [2715](https://github.com/DeusData/codebase-memory-mcp/pull/2715) | feat(watcher): add the Subversion working-copy observation core | OPEN | 隨上游合併後同步 |
| [2714](https://github.com/DeusData/codebase-memory-mcp/pull/2714) | feat(hooks): name the graph follow-up tools in every search augmentation | MERGED | 已含於本次同步（`10ab613`） |
| [2713](https://github.com/DeusData/codebase-memory-mcp/pull/2713) | fix(python): a top-level call of a harmless same-module function keeps the package namespace | MERGED | 已含於本次同步（`10ab613`） |
| [2712](https://github.com/DeusData/codebase-memory-mcp/pull/2712) | fix(python): docstrings and __future__ imports are no namespace barriers | MERGED | 已含於本次同步（`10ab613`） |
| [2710](https://github.com/DeusData/codebase-memory-mcp/pull/2710) | fix(php): drop the name guess when the receiver's class is not indexed | MERGED | 已含於本次同步（`10ab613`） |
| [2708](https://github.com/DeusData/codebase-memory-mcp/pull/2708) | feat(mcp): state the structural questions in the scout and analysis profiles | MERGED | 已含於本次同步（`10ab613`） |
| [2707](https://github.com/DeusData/codebase-memory-mcp/pull/2707) | fix(routes): scope DATA_FLOWS pairing at the test-file boundary (R1 + R2) | MERGED | 已含於本次同步（`10ab613`） |
| [2706](https://github.com/DeusData/codebase-memory-mcp/pull/2706) | fix(php): index trait methods by receiver while flattening (#1265) | MERGED | 已含於本次同步（`10ab613`） |
| [2705](https://github.com/DeusData/codebase-memory-mcp/pull/2705) | fix(php): prove an unaliased global class use absent from the project | MERGED | 已含於本次同步（`10ab613`） |
| [2704](https://github.com/DeusData/codebase-memory-mcp/pull/2704) | fix(python): resolve names re-exported through package namespaces | MERGED | 已含於本次同步（`10ab613`） |
| [2703](https://github.com/DeusData/codebase-memory-mcp/pull/2703) | fix(go-lsp): type variables bound by cross-file multi-value and named results | MERGED | 已含於本次同步（`10ab613`） |
| [2702](https://github.com/DeusData/codebase-memory-mcp/pull/2702) | fix(php): index the single-file LSP type registry | MERGED | 已含於本次同步（`10ab613`） |
| [2700](https://github.com/DeusData/codebase-memory-mcp/pull/2700) | fix(python): resolve annotations through TYPE_CHECKING imports (#1277) | MERGED | 已含於本次同步（`10ab613`） |
| [2698](https://github.com/DeusData/codebase-memory-mcp/pull/2698) | feat(mcp): load the structural tools upfront and say what the graph answers | MERGED | 已含於本次同步（`10ab613`） |
| [2697](https://github.com/DeusData/codebase-memory-mcp/pull/2697) | fix(python-lsp): look attributes up along the C3 method resolution order | MERGED | 已含於本次同步（`10ab613`） |
| [2696](https://github.com/DeusData/codebase-memory-mcp/pull/2696) | fix(python-lsp): resolve super() through the C3 method resolution order | MERGED | 已含於本次同步（`10ab613`） |
| [2695](https://github.com/DeusData/codebase-memory-mcp/pull/2695) | fix(python): bind a nested class base to an earlier sibling of its class body | MERGED | 已含於本次同步（`10ab613`） |
| [2694](https://github.com/DeusData/codebase-memory-mcp/pull/2694) | fix(cli): bound emitted OpenCode augment() with SIGKILL timeout and stdin guard | MERGED | 已含於本次同步（`10ab613`） |
| [2691](https://github.com/DeusData/codebase-memory-mcp/pull/2691) | fix(python): walk a class nested in a class under its outer class | MERGED | 已含於本次同步（`10ab613`） |
| [2690](https://github.com/DeusData/codebase-memory-mcp/pull/2690) | fix(python): extract a decorated class nested in a class body | MERGED | 已含於本次同步（`10ab613`） |
| [2689](https://github.com/DeusData/codebase-memory-mcp/pull/2689) | fix(pipeline): DATA_FLOWS link every caller and handler of a route | MERGED | 已含於本次同步（`10ab613`） |
| [2688](https://github.com/DeusData/codebase-memory-mcp/pull/2688) | fix(routes): extract ASP.NET Core attribute routes (#1692) | MERGED | 已含於本次同步（`10ab613`） |
| [2687](https://github.com/DeusData/codebase-memory-mcp/pull/2687) | fix(pipeline): size definition properties for every field | MERGED | 已含於本次同步（`10ab613`） |
| [2686](https://github.com/DeusData/codebase-memory-mcp/pull/2686) | fix(python): extract defs nested in a function body and resolve bare names lexically (#1548) | MERGED | 已含於本次同步（`10ab613`） |
| [2685](https://github.com/DeusData/codebase-memory-mcp/pull/2685) | fix(rust): generic impl methods source their own calls (#763 Rust share) | MERGED | 已含於本次同步（`10ab613`） |
| [2684](https://github.com/DeusData/codebase-memory-mcp/pull/2684) | fix(test-impact): let a build drop the suites only some runners register | MERGED | 已含於本次同步（`10ab613`） |
| [2682](https://github.com/DeusData/codebase-memory-mcp/pull/2682) | fix(kotlin): resolve calls through typed class properties (#1555) | MERGED | 已含於本次同步（`10ab613`） |
| [2681](https://github.com/DeusData/codebase-memory-mcp/pull/2681) | fix(rust-lsp): Rust calls never bind definitions of other languages | MERGED | 已含於本次同步（`10ab613`） |
| [2680](https://github.com/DeusData/codebase-memory-mcp/pull/2680) | fix(python): from . import x names the importing package, not its parent | MERGED | 已含於本次同步（`10ab613`） |
| [2679](https://github.com/DeusData/codebase-memory-mcp/pull/2679) | fix(python): resolve a base class spelled through a root-binding import | MERGED | 已含於本次同步（`10ab613`） |
| [2678](https://github.com/DeusData/codebase-memory-mcp/pull/2678) | fix(pipeline): link methods to their type after every file is registered | MERGED | 已含於本次同步（`10ab613`） |
| [2677](https://github.com/DeusData/codebase-memory-mcp/pull/2677) | feat(deploy): render Helm images, mark default-valued images, link crds and werf charts | MERGED | 已含於本次同步（`10ab613`） |
| [2676](https://github.com/DeusData/codebase-memory-mcp/pull/2676) | ci(test-impact): make PR test selection narrow for real | MERGED | 已含於本次同步（`10ab613`） |
| [2675](https://github.com/DeusData/codebase-memory-mcp/pull/2675) | Fix build against musl | OPEN | 隨上游合併後同步 |
| [2674](https://github.com/DeusData/codebase-memory-mcp/pull/2674) | chore(pkg): sync packaging metadata to v0.12.1 | MERGED | 已含於本次同步（`10ab613`） |
| [2669](https://github.com/DeusData/codebase-memory-mcp/pull/2669) | fix(ci): give the wrapper smoke a Go toolchain and a deletable work tree | MERGED | 已含於本次同步（`10ab613`） |
| [2667](https://github.com/DeusData/codebase-memory-mcp/pull/2667) | feat(java): no name guess for calls on a proven-external receiver | OPEN | 隨上游合併後同步 |
| [2664](https://github.com/DeusData/codebase-memory-mcp/pull/2664) | fix(pkg): remove the archive size caps from the npm, PyPI and Go wrappers | MERGED | 已含於本次同步（`10ab613`） |
| [2663](https://github.com/DeusData/codebase-memory-mcp/pull/2663) | feat(k8s): a Helm chart deploys its objects, images and values | MERGED | 已含於本次同步（`10ab613`） |

## Issue

| # | 標題 | 狀態 | 決定 |
|---:|---|---|---|
| [2773](https://github.com/DeusData/codebase-memory-mcp/issues/2773) | Direct same-file calls to static functions produce zero CALLS edges (empty trace_path callees) | OPEN | 同檔 static 函式呼叫沒有 CALLS 邊（標 windows，內容平台中立），隨上游 |
| [2768](https://github.com/DeusData/codebase-memory-mcp/issues/2768) | Angular HttpClient calls can be linked to the wrong cross-repo route | OPEN | 平台中立，隨上游 |
| [2763](https://github.com/DeusData/codebase-memory-mcp/issues/2763) | v0.12.0/v0.12.1 Linux release binaries SIGSEGV at startup on glibc 2.44 — bundled free() dereferences its segment table before init | OPEN | Linux 發行檔在 glibc 2.44 啟動崩潰，與 Windows 無關；上游 PR 2731 處理中，隨上游 |
| [2743](https://github.com/DeusData/codebase-memory-mcp/issues/2743) | Watcher loses its successful baseline across unwatch/watch | OPEN | Windows：unwatch／watch 後 watcher 遺失基準；與本 fork 的 watcher 磁碟機檢查相鄰但不同，追蹤，上游處理後同步 |
| [2742](https://github.com/DeusData/codebase-memory-mcp/issues/2742) | Cypher: return zero for ungrouped count(*) over empty input | OPEN | 平台中立，隨上游 |
| [2733](https://github.com/DeusData/codebase-memory-mcp/issues/2733) | auto_watch misses successful index_repository targets from another session root | OPEN | 平台中立，隨上游 |
| [2728](https://github.com/DeusData/codebase-memory-mcp/issues/2728) | Support the 2026-07-28 MCP protocol revision (follow-up to #2433) | OPEN | 平台中立，隨上游 |
| [2725](https://github.com/DeusData/codebase-memory-mcp/issues/2725) | ingest_traces` reports `pairs_unmatched` for endpoints that `query_graph` serves; first large batch stores, all subsequent calls unmatched (fresh index/daemon reproduce) | OPEN | `ingest_traces` 配對失敗（標 windows），追蹤，隨上游 |
| [2723](https://github.com/DeusData/codebase-memory-mcp/issues/2723) | Kotlin/Java: Retrofit client interfaces are minted as server `Route` nodes instead of outbound `HTTP_CALLS` | OPEN | Kotlin／Java Retrofit 介面被當成伺服器 Route（標 windows，內容平台中立），隨上游 |
| [2722](https://github.com/DeusData/codebase-memory-mcp/issues/2722) | Wrong Binary Path Used during Installation | OPEN | Windows：安裝時使用錯誤的執行檔路徑；本機未重現，追蹤，上游處理後同步 |
| [2718](https://github.com/DeusData/codebase-memory-mcp/issues/2718) | TypeScript: a method call on an object imported through a re-export barrel produces no CALLS edge | OPEN | 平台中立，隨上游 |
| [2711](https://github.com/DeusData/codebase-memory-mcp/issues/2711) | [Bug] v0.12.1 Linux: parallel_extract reaches 218% of memory budget before fail_whole; watcher repeatedly retries full reindex | OPEN | 平台中立，隨上游 |
| [2709](https://github.com/DeusData/codebase-memory-mcp/issues/2709) | TypeScript: "who uses X" returns 0 callers or only the definition for constants, fields, types and string values, so agents fall back to grep (field data from 135 sessions) | OPEN | TypeScript「誰使用 X」漏結果（標 windows，內容平台中立），隨上游 |
| [2701](https://github.com/DeusData/codebase-memory-mcp/issues/2701) | Auto-index skipped for any repo containing a symlink (misreported as unsafe_or_unavailable_path, files=-1) | CLOSED | 上游 PR 2720 已修正，已含於本次同步 |
| [2693](https://github.com/DeusData/codebase-memory-mcp/issues/2693) | Emitted OpenCode plugin: hook-augment spawn has no timeout; a hung child blocks every grep/glob/read result | CLOSED | 上游 PR 2694 已修正，已含於本次同步 |
| [2692](https://github.com/DeusData/codebase-memory-mcp/issues/2692) | Issues in MCP implementation | OPEN | 平台中立，隨上游 |
| [2683](https://github.com/DeusData/codebase-memory-mcp/issues/2683) | hook-augment: prove the hook's build identity without hashing the whole executable in every hook process | OPEN | Windows：hook 每次雜湊整個執行檔的效能，隨上游 |
| [2673](https://github.com/DeusData/codebase-memory-mcp/issues/2673) | macOS: two per-conversation servers run duplicate full index_repository workers on the same repo simultaneously (~400% CPU each, respawning every few minutes) — per-repo worker serialization still missing (0.9.0) | OPEN | 平台中立，隨上游 |
| [2672](https://github.com/DeusData/codebase-memory-mcp/issues/2672) | Feature request: project-level codebase-memory integration | OPEN | 平台中立，隨上游 |
| [2671](https://github.com/DeusData/codebase-memory-mcp/issues/2671) | install.sh aborts when agent harness tool config is a symlink instead of update the config | OPEN | 平台中立，隨上游 |
| [2670](https://github.com/DeusData/codebase-memory-mcp/issues/2670) | Lens chat: allow an OpenAI-compatible endpoint (Ollama / llama.cpp / vLLM) instead of in-browser models | OPEN | 平台中立，隨上游 |
| [2668](https://github.com/DeusData/codebase-memory-mcp/issues/2668) | Java: outbound HTTP calls through an injected client, a format-string URL or a request builder yield no `HTTP_CALLS` | OPEN | 平台中立，隨上游 |
| [2666](https://github.com/DeusData/codebase-memory-mcp/issues/2666) | C extraction: a macro-argument identifier becomes a USAGE edge to a same-named struct field | OPEN | 平台中立，隨上游 |
| [2665](https://github.com/DeusData/codebase-memory-mcp/issues/2665) | Smart CI: tell a Makefile.cbm / test_main.c registration from a build or harness change | OPEN | 平台中立，隨上游 |
| [2662](https://github.com/DeusData/codebase-memory-mcp/issues/2662) | Java: index Lombok-generated members, so calls to them resolve by type | OPEN | 平台中立，隨上游 |
| [2661](https://github.com/DeusData/codebase-memory-mcp/issues/2661) | A Java package named target, tmp, deploy or vendor is silently not indexed | OPEN | 平台中立，隨上游 |
| [2660](https://github.com/DeusData/codebase-memory-mcp/issues/2660) | Java: a call on a JDK-typed receiver binds a same-named project method | OPEN | 平台中立，隨上游 |
| [2659](https://github.com/DeusData/codebase-memory-mcp/issues/2659) | Python: `module.func()` calls through a module loaded with `importlib.util.spec_from_file_location` are no longer traced (0.10.8 → 0.11.0, still in 0.12.0) | OPEN | 平台中立，隨上游 |

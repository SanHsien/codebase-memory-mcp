# 上游 issue／PR 分流（2026-09-29）

範圍：`DeusData/codebase-memory-mcp` 全部 **open** 項目，共 647 筆（issue 474、PR 173）。全量清單見 [`upstream-triage-2026-09-29.tsv`](upstream-triage-2026-09-29.tsv)。

**方法與限制（誠實聲明）**：這是**規則式分流**，依 GitHub 標籤與標題關鍵字歸類，**沒有逐筆讀程式碼或重現問題**。本機沒有 C 工具鏈，也無法驗證修復。「main 有 commit 提及」只代表 `origin/main` 的 commit 訊息出現過該編號，不等於已修好，需另行確認。

**處理原則**：本 fork 不在上游留言、關閉或開 PR（未經當次對話同意回貢，見 [`FORK.md`](../../FORK.md)）。分流結果只記在本 repo；Windows 與安全性項目是本 fork 之後優先跟進的清單。

## 總覽

| 類別 | Issue | PR | 決定 |
|---|---:|---:|---|
| windows | 109 | 9 | Windows（本 fork 主要環境，要跟） |
| security | 25 | 14 | 安全性（要跟） |
| deps | 0 | 7 | Dependabot 流程 |
| neutral | 292 | 142 | 平台中立（隨上游） |
| other-platform | 7 | 1 | 非 Windows 平台（不主動處理） |
| waiting | 41 | 0 | 等回報者（不處理） |

另有 62 筆 issue 在 `origin/main` 的 commit 訊息中被提及（`main_has_commit_ref` 欄），可能已修，待驗證。

## Windows（本 fork 主要環境，要跟）：PR（9）

| # | 標題 | main 提及 | 建立 |
|---:|---|:-:|---|
| [2381](https://github.com/DeusData/codebase-memory-mcp/pull/2381) | fix(cli): resolve Windows CLI detection for long PATH and npm/mise/scoop shims (#221) |  | 2026-09-27 |
| [2290](https://github.com/DeusData/codebase-memory-mcp/pull/2290) | fix(windows): set UTF-8 environment values through wide CRT |  | 2026-09-22 |
| [2281](https://github.com/DeusData/codebase-memory-mcp/pull/2281) | fix(windows): use matching deallocator for CRT aligned blocks |  | 2026-09-22 |
| [2262](https://github.com/DeusData/codebase-memory-mcp/pull/2262) | Docs/windows troubleshooting |  | 2026-09-20 |
| [2251](https://github.com/DeusData/codebase-memory-mcp/pull/2251) | fix(install.ps1): reserve temp paths exclusively instead of adopting them |  | 2026-09-20 |
| [2244](https://github.com/DeusData/codebase-memory-mcp/pull/2244) | fix(cli): support non-ASCII Windows agent paths |  | 2026-09-20 |
| [2217](https://github.com/DeusData/codebase-memory-mcp/pull/2217) | test(windows): guard clean MCP stdio startup |  | 2026-09-15 |
| [2213](https://github.com/DeusData/codebase-memory-mcp/pull/2213) | test(windows): guard clean MCP stdio startup |  | 2026-09-15 |
| [1823](https://github.com/DeusData/codebase-memory-mcp/pull/1823) | fix(ci): stop kill-grace from bounding Windows helper spawns |  | 2026-08-24 |

## Windows（本 fork 主要環境，要跟）：Issue（109）

| # | 標題 | main 提及 | 建立 |
|---:|---|:-:|---|
| [2400](https://github.com/DeusData/codebase-memory-mcp/issues/2400) | Multiple codebase memory .exe running |  | 2026-09-28 |
| [2399](https://github.com/DeusData/codebase-memory-mcp/issues/2399) | persist_failed on very large repositories: staging DB dump stops at exactly 4 GiB (Windows, v0.11.0) |  | 2026-09-28 |
| [2382](https://github.com/DeusData/codebase-memory-mcp/issues/2382) | Java: a call on an explicitly-typed parameter binds to a same-named method of an unrelated class |  | 2026-09-27 |
| [2379](https://github.com/DeusData/codebase-memory-mcp/issues/2379) | Swift: initializers are not extracted — calls inside init are attributed to the file |  | 2026-09-26 |
| [2377](https://github.com/DeusData/codebase-memory-mcp/issues/2377) | Swift: vendored grammar predates SE-0458 unsafe expressions and typed-throws initializers — files index partially and lose CALLS edges |  | 2026-09-26 |
| [2316](https://github.com/DeusData/codebase-memory-mcp/issues/2316) | Optionally follow approved symlinks / Windows junctions during repository indexing |  | 2026-09-25 |
| [2291](https://github.com/DeusData/codebase-memory-mcp/issues/2291) | Cross-repository intelligence does not detect dynamic BFF-to-backend connections |  | 2026-09-23 |
| [2276](https://github.com/DeusData/codebase-memory-mcp/issues/2276) | Artifact (`.codebase-memory/graph.db.zst`) cannot be reused across directories of the same repo — project name, `root_path`, and session detection are all path-bound |  | 2026-09-22 |
| [2265](https://github.com/DeusData/codebase-memory-mcp/issues/2265) | trace_path reports callers_total: 0 with relation: eq for injected-receiver calls; the resolver already records the unresolved site but drops it before any output |  | 2026-09-21 |
| [2250](https://github.com/DeusData/codebase-memory-mcp/issues/2250) | query_graph: `WHERE NOT <predicate>` on the target node or relationship of a MATCH pattern silently returns 0 rows — same predicate with `<>`, inside `OR`, or after `WITH` returns the right rows |  | 2026-09-20 |
| [2226](https://github.com/DeusData/codebase-memory-mcp/issues/2226) | Control Panel renders cumulative CPU-seconds as a percentage — "Total CPU" grows without bound (0.11.0, Windows) |  | 2026-09-16 |
| [2211](https://github.com/DeusData/codebase-memory-mcp/issues/2211) | Indexing gap: 1C.pas using Cyrillic identifiers |  | 2026-09-15 |
| [2209](https://github.com/DeusData/codebase-memory-mcp/issues/2209) | install on Hermes Agent: pre_llm_hook_install fails (mcp_servers + skill succeed) |  | 2026-09-15 |
| [2208](https://github.com/DeusData/codebase-memory-mcp/issues/2208) | query_graph: property access on a WITH-carried (in-scope) node returns wrong values, not the real property |  | 2026-09-14 |
| [2184](https://github.com/DeusData/codebase-memory-mcp/issues/2184) | Indexing could not be completed |  | 2026-09-12 |
| [2177](https://github.com/DeusData/codebase-memory-mcp/issues/2177) | GitHub Copilot CLI: PreToolUse/PostToolUse hook augmentation not implemented (only sessionStart/subagentStart wired up) |  | 2026-09-11 |
| [2174](https://github.com/DeusData/codebase-memory-mcp/issues/2174) | Indexing gap: scripts/windows/pi-web-tray.ps1 |  | 2026-09-11 |
| [2167](https://github.com/DeusData/codebase-memory-mcp/issues/2167) | [0.10.8/Windows] Background watcher never picks up committed new files (120s+) |  | 2026-09-10 |
| [2165](https://github.com/DeusData/codebase-memory-mcp/issues/2165) | NestJS: @Controller/@Get/@Post routes are not extracted into Route nodes (decorator text is already captured) |  | 2026-09-10 |
| [2145](https://github.com/DeusData/codebase-memory-mcp/issues/2145) | Windows: `install` reports "Detected agents: (none)" when the profile path contains non-ASCII characters — agent detection still uses narrow stat() (v0.10.8) |  | 2026-09-10 |
| [2144](https://github.com/DeusData/codebase-memory-mcp/issues/2144) | Failed to index rdue to timeout |  | 2026-09-10 |
| [2093](https://github.com/DeusData/codebase-memory-mcp/issues/2093) | Idle MCP frontend still burns ~19% of a core on v0.10.8 (Windows) — looks like a regression of #1764 |  | 2026-09-07 |
| [2088](https://github.com/DeusData/codebase-memory-mcp/issues/2088) | Index repository failed |  | 2026-09-07 |
| [2071](https://github.com/DeusData/codebase-memory-mcp/issues/2071) | C# 14 extension declarations can cause file to be omitted from index |  | 2026-09-06 |
| [2058](https://github.com/DeusData/codebase-memory-mcp/issues/2058) | hook-augment restarts full daemon on every invocation, causing ~3s latency per call and hook timeouts |  | 2026-09-04 |
| [2057](https://github.com/DeusData/codebase-memory-mcp/issues/2057) | test-windows-guards: test_daemon_stability turns setup failures and timeouts into REGRESSION verdicts on unrelated PRs |  | 2026-09-04 |
| [2045](https://github.com/DeusData/codebase-memory-mcp/issues/2045) | Windows 11 - Installation CBM Deletes all user path variables except itself |  | 2026-09-04 |
| [2044](https://github.com/DeusData/codebase-memory-mcp/issues/2044) | Windows v0.10.8 installer fails while configuring multiple agents |  | 2026-09-04 |
| [2021](https://github.com/DeusData/codebase-memory-mcp/issues/2021) | Windows BSOD (0x3B) in Ntfs.sys repeatedly triggered during codebase-memory-mcp file-name queries under multi-session load |  | 2026-09-03 |
| [2007](https://github.com/DeusData/codebase-memory-mcp/issues/2007) | Indexing gap: C-Users-hp-AndroidStudioProjects (UTF-16 JSON + POSIX shell) |  | 2026-09-02 |
| [1951](https://github.com/DeusData/codebase-memory-mcp/issues/1951) | detect_changes loses impacted symbols when the indexed project is a subdirectory of a normal Git repository |  | 2026-08-30 |
| [1948](https://github.com/DeusData/codebase-memory-mcp/issues/1948) | Non-git silent on staleness |  | 2026-08-30 |
| [1917](https://github.com/DeusData/codebase-memory-mcp/issues/1917) | Windows: mtime encoding split makes the incremental classifier see every file as changed |  | 2026-08-29 |
| [1916](https://github.com/DeusData/codebase-memory-mcp/issues/1916) | cross-repo-intelligence: HTTP_CALLS edges not detected for axios wrapper pattern |  | 2026-08-29 |
| [1887](https://github.com/DeusData/codebase-memory-mcp/issues/1887) | Two MINGW64_NT platform guards in smoke-test.sh never skip (known #929 bug, fixed in one place only) |  | 2026-08-28 |
| [1829](https://github.com/DeusData/codebase-memory-mcp/issues/1829) | `/api/ui-config` ignores `Accept-Language` ordering and q-values — returns `zh` if Chinese appears anywhere in the list |  | 2026-08-25 |
| [1827](https://github.com/DeusData/codebase-memory-mcp/issues/1827) | Non-ASCII (e.g. Chinese) directory paths produce an irreversible hex-encoded, non-discoverable project identifier |  | 2026-08-25 |
| [1818](https://github.com/DeusData/codebase-memory-mcp/issues/1818) | daemon.autoindex.skipped omits the effective auto_index_limit |  | 2026-08-24 |
| [1797](https://github.com/DeusData/codebase-memory-mcp/issues/1797) | i have install using repo's step but it only recognize gemini or antigravity cli , it does work on antigravity 2.0 does anyone know how to make sure it work on Antigravity 2.0 as well also guide me also |  | 2026-08-22 |
| [1748](https://github.com/DeusData/codebase-memory-mcp/issues/1748) | C# collection expression in a ternary branch fails to parse (reported as parse_partial) |  | 2026-08-19 |
| [1744](https://github.com/DeusData/codebase-memory-mcp/issues/1744) | Upgrade to a new version doesn't preserve settings |  | 2026-08-19 |
| [1732](https://github.com/DeusData/codebase-memory-mcp/issues/1732) | pnpm workspace: no IMPORTS edge when package.json entry points at gitignored dist/ — cross-package CALLS degrade to unique_name |  | 2026-08-19 |
| [1720](https://github.com/DeusData/codebase-memory-mcp/issues/1720) | Windows installation issue |  | 2026-08-19 |
| [1718](https://github.com/DeusData/codebase-memory-mcp/issues/1718) | `--approve-sensitive` never lifts the sensitive-root refusal for Windows paths under `Program Files` |  | 2026-08-18 |
| [1693](https://github.com/DeusData/codebase-memory-mcp/issues/1693) | manage_adr returns "project not found or not indexed" for a valid project while all sibling tools resolve it (v0.10.5) |  | 2026-08-17 |
| [1692](https://github.com/DeusData/codebase-memory-mcp/issues/1692) | C#: log calls inside Kafka-consumer classes misclassified as Route nodes; ASP.NET Core [Route]/[HttpGet] attribute routes not extracted (cross-repo-intelligence returns 0 edges) | 是 | 2026-08-17 |
| [1682](https://github.com/DeusData/codebase-memory-mcp/issues/1682) | TypeScript: CALLS edges silently missing for some directly-imported functions — trace_path reports callers_total: 0 (v0.10.5) |  | 2026-08-16 |
| [1599](https://github.com/DeusData/codebase-memory-mcp/issues/1599) | install: no way to skip a single agent, so VS Code gets an absolute path written into every profile |  | 2026-08-13 |
| [1582](https://github.com/DeusData/codebase-memory-mcp/issues/1582) | Claude.ai Desktop App still cannot start mcp | 是 | 2026-08-13 |
| [1581](https://github.com/DeusData/codebase-memory-mcp/issues/1581) | Source directory named 'deploy' is silently skipped as build output, so the graph returns the wrong definition |  | 2026-08-13 |
| [1572](https://github.com/DeusData/codebase-memory-mcp/issues/1572) | Python stdlib import binds to a same-named TypeScript function (cross-language, strategy=unique_name) — poisons hotspots and Louvain clusters | 是 | 2026-08-12 |
| [1570](https://github.com/DeusData/codebase-memory-mcp/issues/1570) | Issue with Project Indexing in codebase-memory-mcp v0.10.2 |  | 2026-08-12 |
| [1565](https://github.com/DeusData/codebase-memory-mcp/issues/1565) | Search performance and search cancellation |  | 2026-08-12 |
| [1559](https://github.com/DeusData/codebase-memory-mcp/issues/1559) | index_status(verbose) reports the live repo head_sha, not the indexed generation — misleading freshness signal |  | 2026-08-12 |
| [1557](https://github.com/DeusData/codebase-memory-mcp/issues/1557) | Kotlin interface nodes get label Class; companion-object members and TS re-export aliases produce no nodes |  | 2026-08-12 |
| [1556](https://github.com/DeusData/codebase-memory-mcp/issues/1556) | TESTS_FILE edges are never created for JVM (Kotlin/JUnit) tests |  | 2026-08-12 |
| [1555](https://github.com/DeusData/codebase-memory-mcp/issues/1555) | Call resolution picks the wrong target (or none) for common method names in a Kotlin monorepo |  | 2026-08-12 |
| [1548](https://github.com/DeusData/codebase-memory-mcp/issues/1548) | Python: defs nested inside a function are never extracted — app-factory (create_app) codebases lose their entire HTTP layer, and inbound traces silently return zero callers |  | 2026-08-11 |
| [1546](https://github.com/DeusData/codebase-memory-mcp/issues/1546) | C/C++: a CRLF file with a backslash-continued string literal swallows the rest of the file, and every later function is missing from the graph |  | 2026-08-11 |
| [1478](https://github.com/DeusData/codebase-memory-mcp/issues/1478) | CBM_WATCH_MAX_INTERVAL_MS env var to configure watcher poll interval cap |  | 2026-08-06 |
| [1428](https://github.com/DeusData/codebase-memory-mcp/issues/1428) | NestJS: @Controller/@Get decorator routes not extracted - 0 Route nodes, so cross-repo-intelligence finds no edges |  | 2026-08-04 |
| [1407](https://github.com/DeusData/codebase-memory-mcp/issues/1407) | C# overloaded methods not indexed |  | 2026-08-01 |
| [1311](https://github.com/DeusData/codebase-memory-mcp/issues/1311) | Error accessing other projects |  | 2026-07-28 |
| [1209](https://github.com/DeusData/codebase-memory-mcp/issues/1209) | [v0.9.0][Windows] search_code content pipeline transcodes UTF-8 through the ANSI codepage — CJK pattern search always returns 0 matches, raw_matches show mojibake |  | 2026-07-22 |
| [1189](https://github.com/DeusData/codebase-memory-mcp/issues/1189) | after index a java project, ui slow |  | 2026-07-20 |
| [1185](https://github.com/DeusData/codebase-memory-mcp/issues/1185) | Server doesn't advertise tools capability when client sends empty capabilities in initialize |  | 2026-07-20 |
| [1182](https://github.com/DeusData/codebase-memory-mcp/issues/1182) | Windows/Codex: MCP tool calls close transport while CLI/stdio init work; update -y still prompts for binary variant |  | 2026-07-20 |
| [1180](https://github.com/DeusData/codebase-memory-mcp/issues/1180) | The install script failed to detect Hermes(Desktop) and OpenCode(Desktop) on Windows |  | 2026-07-19 |
| [1174](https://github.com/DeusData/codebase-memory-mcp/issues/1174) | [Windows] Repeated incremental re-index leaves an oversized/stuck SQLite WAL — node count frozen while HEAD pointer advances; delete_project returns "Permission denied" |  | 2026-07-19 |
| [1171](https://github.com/DeusData/codebase-memory-mcp/issues/1171) | [v0.9.0][Windows] persistence=true crashes the indexing worker for repositories under CJK paths |  | 2026-07-19 |
| [1170](https://github.com/DeusData/codebase-memory-mcp/issues/1170) | docs: add a Windows known-issues / troubleshooting section |  | 2026-07-18 |
| [1167](https://github.com/DeusData/codebase-memory-mcp/issues/1167) | fails to install for opencode on windows (other os's unknown) |  | 2026-07-18 |
| [1165](https://github.com/DeusData/codebase-memory-mcp/issues/1165) | Windows: CBM_CACHE_DIR / CBM_ALLOWED_ROOT are lossily decoded — every non-ASCII char becomes U+FFFD, so the server silently uses a different path (v0.9.0) |  | 2026-07-18 |
| [1161](https://github.com/DeusData/codebase-memory-mcp/issues/1161) | Installer targets legacy Gemini-CLI settings.json instead of Antigravity 2.0 config directory (resulting in duplicate 273MB binaries) |  | 2026-07-18 |
| [1145](https://github.com/DeusData/codebase-memory-mcp/issues/1145) | Windows-Indexing worker crashed on a file | 是 | 2026-07-17 |
| [1133](https://github.com/DeusData/codebase-memory-mcp/issues/1133) | cross-repo-intelligence: --target-projects flag silently parses to zero targets; with raw-JSON args the worker crashes (0-byte log) | 是 | 2026-07-16 |
| [1132](https://github.com/DeusData/codebase-memory-mcp/issues/1132) | Indexing worker can freeze the whole OS on binary-heavy folders, and always dies with a 0-byte log — needs resource guards and early log flush | 是 | 2026-07-16 |
| [1130](https://github.com/DeusData/codebase-memory-mcp/issues/1130) | Worker continuously hangs and indexing process gives up eventually | 是 | 2026-07-16 |
| [1120](https://github.com/DeusData/codebase-memory-mcp/issues/1120) | Add SVN operation acceptance coverage |  | 2026-07-16 |
| [1117](https://github.com/DeusData/codebase-memory-mcp/issues/1117) | Windows cached stores can block watcher publication |  | 2026-07-16 |
| [1084](https://github.com/DeusData/codebase-memory-mcp/issues/1084) | CPU memory usage is too high |  | 2026-07-14 |
| [1083](https://github.com/DeusData/codebase-memory-mcp/issues/1083) | WAL grows unbounded (115 GB in 4.5 h, fills system drive): checkpoint starvation from accumulated per-conversation servers + 11 concurrent index workers on one repo | 是 | 2026-07-14 |
| [1081](https://github.com/DeusData/codebase-memory-mcp/issues/1081) | C# files with CRLF line endings + XML doc comments (///) silently parse to zero Method/Class nodes (v0.9.0, Windows) |  | 2026-07-14 |
| [1073](https://github.com/DeusData/codebase-memory-mcp/issues/1073) | Support VB.NET (.vb) language — files skipped during indexing |  | 2026-07-13 |
| [1070](https://github.com/DeusData/codebase-memory-mcp/issues/1070) | Index Error: outcome=killed exit_code=-1 signal=9 | 是 | 2026-07-13 |
| [1041](https://github.com/DeusData/codebase-memory-mcp/issues/1041) | [Feature/Bug] ASP.NET Core attribute routing ([HttpGet], [HttpPost], [Route]) not extracted — server-side Route nodes missing for controller-based .NET APIs |  | 2026-07-12 |
| [965](https://github.com/DeusData/codebase-memory-mcp/issues/965) | v0.9.0 Windows: search_code times out after 180s; related parallel call shows ~180s dispatch wait; diagnostics lack per-call timing |  | 2026-07-08 |
| [935](https://github.com/DeusData/codebase-memory-mcp/issues/935) | how to connct this to opencode (windows)? |  | 2026-07-07 |
| [909](https://github.com/DeusData/codebase-memory-mcp/issues/909) | Standalone dashboard (--ui=true) dies silently within seconds, no stderr output |  | 2026-07-06 |
| [903](https://github.com/DeusData/codebase-memory-mcp/issues/903) | Windows: get_code_snippet returns "(source not available)" and search_code returns 0 matches for projects indexed under non-ASCII (CJK) paths — indexing itself succeeds |  | 2026-07-06 |
| [850](https://github.com/DeusData/codebase-memory-mcp/issues/850) | flaky: probe_clojure_imports_edge intermittently fails on Windows (0 imports) |  | 2026-07-04 |
| [841](https://github.com/DeusData/codebase-memory-mcp/issues/841) | watcher: continuous auto-sync re-indexing churn on Windows (real driver of #832) | 是 | 2026-07-04 |
| [795](https://github.com/DeusData/codebase-memory-mcp/issues/795) | codebase-memory-mcp-ui-windows-amd64 doesn't provide ui function |  | 2026-07-03 |
| [708](https://github.com/DeusData/codebase-memory-mcp/issues/708) | index not working |  | 2026-06-30 |
| [654](https://github.com/DeusData/codebase-memory-mcp/issues/654) | [Windows] MCP stdio startup emits localized path-not-found message on stderr |  | 2026-06-27 |
| [635](https://github.com/DeusData/codebase-memory-mcp/issues/635) | Windows: MCP server hangs on initialize when launched by Hermes/AnyIO stdio client | 是 | 2026-06-26 |
| [627](https://github.com/DeusData/codebase-memory-mcp/issues/627) | Crash when calling query_graph | 是 | 2026-06-25 |
| [595](https://github.com/DeusData/codebase-memory-mcp/issues/595) | 📌 Epics / Roadmap — umbrella task index | 是 | 2026-06-23 |
| [594](https://github.com/DeusData/codebase-memory-mcp/issues/594) | task: Graph queries & trace_path resolution |  | 2026-06-23 |
| [593](https://github.com/DeusData/codebase-memory-mcp/issues/593) | task: Performance & memory on large repositories |  | 2026-06-23 |
| [592](https://github.com/DeusData/codebase-memory-mcp/issues/592) | task: Parsing & extraction quality — language/format coverage gaps | 是 | 2026-06-23 |
| [581](https://github.com/DeusData/codebase-memory-mcp/issues/581) | Memory leak: process grows to 50+ GB virtual memory over hours/days, crashes Windows | 是 | 2026-06-23 |
| [530](https://github.com/DeusData/codebase-memory-mcp/issues/530) | Bug Fixes & Improvements: Windows Stdio hangs, Cascading Gitignore, UTF-8 Snippets | 是 | 2026-06-20 |
| [498](https://github.com/DeusData/codebase-memory-mcp/issues/498) | 3D graph visualization freezes browser/system on repos with >10K nodes | 是 | 2026-06-18 |
| [396](https://github.com/DeusData/codebase-memory-mcp/issues/396) | task: UX, install, watcher, docs & small features (15 issues) |  | 2026-05-30 |
| [395](https://github.com/DeusData/codebase-memory-mcp/issues/395) | task: Editor / agent / MCP-client integration (6 issues) |  | 2026-05-30 |
| [394](https://github.com/DeusData/codebase-memory-mcp/issues/394) | task: Windows platform issues (8 bugs) | 是 | 2026-05-30 |
| [390](https://github.com/DeusData/codebase-memory-mcp/issues/390) | task: Stability — crashes, segfaults, and memory safety during indexing (14 issues) | 是 | 2026-05-30 |
| [221](https://github.com/DeusData/codebase-memory-mcp/issues/221) | "install" command does not work for opencode in windows 11 | 是 | 2026-04-07 |

## 安全性（要跟）：PR（14）

| # | 標題 | main 提及 | 建立 |
|---:|---|:-:|---|
| [2214](https://github.com/DeusData/codebase-memory-mcp/pull/2214) | fix(mcp): prune cached projects with missing roots |  | 2026-09-15 |
| [2207](https://github.com/DeusData/codebase-memory-mcp/pull/2207) | fix(daemon): stop the lifetime-lock probe from dropping a held lock |  | 2026-09-14 |
| [2072](https://github.com/DeusData/codebase-memory-mcp/pull/2072) | feat(mcp): add CBM_IN_PROCESS to serve stdio MCP without the daemon |  | 2026-09-06 |
| [2017](https://github.com/DeusData/codebase-memory-mcp/pull/2017) | feat(workspace): add PATH_ALLOW_BROAD override for too-shallow roots |  | 2026-09-02 |
| [1996](https://github.com/DeusData/codebase-memory-mcp/pull/1996) | fix(store,ui): export paired free functions for FFI callers (#1762) |  | 2026-09-01 |
| [1925](https://github.com/DeusData/codebase-memory-mcp/pull/1925) | Add memory-aware concurrent indexing for large repositories |  | 2026-08-30 |
| [1804](https://github.com/DeusData/codebase-memory-mcp/pull/1804) | test.sh: refuse the cli suite against a real HOME |  | 2026-08-22 |
| [1786](https://github.com/DeusData/codebase-memory-mcp/pull/1786) | fix(workspace): treat '/' and '\' as the same separator in root matching (#1718) |  | 2026-08-21 |
| [1781](https://github.com/DeusData/codebase-memory-mcp/pull/1781) | fix(codex): migrate skill to documented user path |  | 2026-08-21 |
| [1649](https://github.com/DeusData/codebase-memory-mcp/pull/1649) | feat(config): make Windows cache-dir DACL hardening configurable (env var + CLI config key) |  | 2026-08-14 |
| [1188](https://github.com/DeusData/codebase-memory-mcp/pull/1188) | Feat/claude plugin |  | 2026-07-20 |
| [1179](https://github.com/DeusData/codebase-memory-mcp/pull/1179) | feat(visualbasic): add VB.NET structural extraction (WinForms-grade) |  | 2026-07-19 |
| [1127](https://github.com/DeusData/codebase-memory-mcp/pull/1127) | feat(watcher): sync SVN working copies automatically |  | 2026-07-16 |
| [808](https://github.com/DeusData/codebase-memory-mcp/pull/808) | feat:Add local personal memory |  | 2026-07-03 |

## 安全性（要跟）：Issue（25）

| # | 標題 | main 提及 | 建立 |
|---:|---|:-:|---|
| [2052](https://github.com/DeusData/codebase-memory-mcp/issues/2052) | Security review request: file-access scope — symlinks and path injection (suspicion of out-of-scope reads) |  | 2026-09-04 |
| [2023](https://github.com/DeusData/codebase-memory-mcp/issues/2023) | Windows: v0.10.8 still rejects CodexSandboxUsers profile ACL during secure coordination | 是 | 2026-09-03 |
| [2014](https://github.com/DeusData/codebase-memory-mcp/issues/2014) | /foo: path is too broad... |  | 2026-09-02 |
| [2003](https://github.com/DeusData/codebase-memory-mcp/issues/2003) | pre-commit hook fails the test suite: git exports GIT_DIR into hooks, which overrides `git -C` in tests/test_pipeline.c |  | 2026-09-02 |
| [1964](https://github.com/DeusData/codebase-memory-mcp/issues/1964) | Atlas companion frontend: integration proposal for #1860 |  | 2026-08-31 |
| [1884](https://github.com/DeusData/codebase-memory-mcp/issues/1884) | feat(incremental): debounce artifact auto re-export |  | 2026-08-28 |
| [1856](https://github.com/DeusData/codebase-memory-mcp/issues/1856) | Windows: activation ancestor-walk fails unconditionally on stock/default ACLs (no OS-level denial in ProcMon trace) | 是 | 2026-08-27 |
| [1734](https://github.com/DeusData/codebase-memory-mcp/issues/1734) | [RFC] Evidence-grade pre-edit and post-edit workflows for coding agents |  | 2026-08-19 |
| [1717](https://github.com/DeusData/codebase-memory-mcp/issues/1717) | Cache directory validation rejects symlinks owned by other user |  | 2026-08-18 |
| [1696](https://github.com/DeusData/codebase-memory-mcp/issues/1696) | Audit smoke, soak, profiling, and benchmark harnesses for account daemon runtime isolation | 是 | 2026-08-17 |
| [1687](https://github.com/DeusData/codebase-memory-mcp/issues/1687) | WSL2: Default Windows drive mounts (/mnt/c, /mnt/d, /mnt/e) fail parent directory permission checks (0777 world-writable) |  | 2026-08-17 |
| [1624](https://github.com/DeusData/codebase-memory-mcp/issues/1624) | feat(config): make Windows cache-dir DACL hardening configurable (env var + CLI config key) — opt-out for hosts where it breaks MoveFileExW |  | 2026-08-14 |
| [1534](https://github.com/DeusData/codebase-memory-mcp/issues/1534) | Feature Suggestion: Optional token metering & paid API key support via `neuforge-pay` |  | 2026-08-11 |
| [1533](https://github.com/DeusData/codebase-memory-mcp/issues/1533) | Windows: ancestor walk refuses app container / capability SIDs, so 0.10.0 cannot start inside a containerized host | 是 | 2026-08-11 |
| [1458](https://github.com/DeusData/codebase-memory-mcp/issues/1458) | Security Review: MCP Server Tool Exposure + Memory Poisoning Vectors |  | 2026-08-05 |
| [1256](https://github.com/DeusData/codebase-memory-mcp/issues/1256) | Feature Request: Native multi-tenancy support and built-in user authentication / access control |  | 2026-07-24 |
| [1218](https://github.com/DeusData/codebase-memory-mcp/issues/1218) | [v0.9.0] install.ps1 expects codebase-memory-mcp.payload.exe missing from release archive |  | 2026-07-23 |
| [1210](https://github.com/DeusData/codebase-memory-mcp/issues/1210) | Allow UI to be exposed on a configurable host (0.0.0.0) for remote/VPS deployments |  | 2026-07-22 |
| [1200](https://github.com/DeusData/codebase-memory-mcp/issues/1200) | Installer replaces the whole SessionStart array in ~/.claude/settings.json, silently deleting unrelated hooks |  | 2026-07-21 |
| [1194](https://github.com/DeusData/codebase-memory-mcp/issues/1194) | Windows Defender report: Win32/Peardis.C |  | 2026-07-21 |
| [1183](https://github.com/DeusData/codebase-memory-mcp/issues/1183) | Windows / Claude - session.root.cwd path used C:\Windows\system32 when auto_index is True |  | 2026-07-20 |
| [1124](https://github.com/DeusData/codebase-memory-mcp/issues/1124) | Make SVN fingerprint opens race-safe |  | 2026-07-16 |
| [1121](https://github.com/DeusData/codebase-memory-mcp/issues/1121) | Expand SVN status and path-safety test matrix |  | 2026-07-16 |
| [1079](https://github.com/DeusData/codebase-memory-mcp/issues/1079) | Feature request: expose .env assignments as graph nodes (name + value + file) |  | 2026-07-13 |
| [1038](https://github.com/DeusData/codebase-memory-mcp/issues/1038) | `uninstall --help` performs a real uninstall — unknown flags silently ignored, no confirmation before destructive removal | 是 | 2026-07-12 |

## 下一步

1. 裝好 MSYS2 CLANG64 後，逐筆驗證 Windows 與安全性項目在 `main` 是否仍可重現。
2. 仍存在者，決定「在 fork 修」或「等上游」，記入 `docs/DECISIONS.md`。
3. 每週 `upstream-check` 會報新增項目；審查後推進 `tools/upstream_baseline.json`。

/*
 * test_stack_overflow.c — Regression tests for GitHub issue #199.
 *
 * Verifies that extraction functions do NOT silently drop AST nodes
 * when files exceed the fixed traversal stack capacity (512 for calls,
 * 256 for variables, 64 for Elixir, etc.).
 *
 * These tests generate source code with more call sites / definitions /
 * imports than the stack cap, then assert the extraction count matches
 * the expected total. Before the fix, counts plateau at the cap.
 */
#include "test_framework.h"
#include "cbm.h"
#include "foundation/platform.h" /* cbm_now_ms */
#include "lang_specs.h" /* cbm_ts_language — direct-parse GLR cap regression (#913) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/* tree-sitter runtime allocator hooks (ts_runtime/src/alloc.h, TS_PUBLIC) and
 * mimalloc (vendored) — for the #424 allocator-binding regression test. */
extern void *(*ts_current_malloc)(size_t);
extern void *(*ts_current_calloc)(size_t, size_t);
extern void *(*ts_current_realloc)(void *, size_t);
extern void (*ts_current_free)(void *);
extern void *mi_malloc(size_t);
extern void *mi_calloc(size_t, size_t);
extern void *mi_realloc(void *, size_t);
extern void mi_free(void *);

/* ── Helpers ───────────────────────────────────────────────────── */

static CBMFileResult *extract(const char *src, CBMLanguage lang, const char *proj,
                              const char *path) {
    CBMFileResult *r = cbm_extract_file(src, (int)strlen(src), lang, proj, path, 0, NULL, NULL);
    return r;
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: JavaScript calls exceeding 512 stack cap
 *
 * Generates a JS function with 600 unique function calls.
 * Before fix: walk_calls() stops at ~512 due to CALLS_STACK_CAP.
 * After fix: all 600 calls are extracted.
 * ═══════════════════════════════════════════════════════════════════ */

TEST(js_calls_exceed_512) {
    const int CALL_COUNT = 600;
    /* Generate calls spread across many small functions to create wide AST.
     * Each function has ~20 calls, 30 functions = 600 calls total.
     * The DFS stack must hold sibling function nodes simultaneously. */
    const int FUNCS = 30;
    const int CALLS_PER = CALL_COUNT / FUNCS;
    size_t buf_sz = 256 + (size_t)CALL_COUNT * 48;
    char *src = malloc(buf_sz);
    ASSERT_NOT_NULL(src);

    char *p = src;
    for (int f = 0; f < FUNCS; f++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "function handler_%d() {\n", f);
        for (int c = 0; c < CALLS_PER; c++) {
            int idx = f * CALLS_PER + c;
            p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "  func_%d();\n", idx);
        }
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "}\n");
    }

    CBMFileResult *r = extract(src, CBM_LANG_JAVASCRIPT, "test", "many_calls.js");
    ASSERT_NOT_NULL(r);
    ASSERT_FALSE(r->has_error);

    /* Count calls that match our generated pattern */
    int matched = 0;
    for (int i = 0; i < r->calls.count; i++) {
        if (strncmp(r->calls.items[i].callee_name, "func_", 5) == 0) {
            matched++;
        }
    }

    printf("    calls extracted: %d / %d expected\n", matched, CALL_COUNT);
    ASSERT_EQ(matched, CALL_COUNT);

    cbm_free_result(r);
    free(src);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: Python calls exceeding 512 stack cap
 *
 * Same test in Python syntax to verify language-independence.
 * ═══════════════════════════════════════════════════════════════════ */

TEST(python_calls_exceed_512) {
    const int CALL_COUNT = 600;
    size_t buf_sz = 256 + (size_t)CALL_COUNT * 32;
    char *src = malloc(buf_sz);
    ASSERT_NOT_NULL(src);

    char *p = src;
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "def main():\n");
    for (int i = 0; i < CALL_COUNT; i++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "    func_%d()\n", i);
    }

    CBMFileResult *r = extract(src, CBM_LANG_PYTHON, "test", "many_calls.py");
    ASSERT_NOT_NULL(r);
    ASSERT_FALSE(r->has_error);

    int matched = 0;
    for (int i = 0; i < r->calls.count; i++) {
        if (strncmp(r->calls.items[i].callee_name, "func_", 5) == 0) {
            matched++;
        }
    }

    printf("    calls extracted: %d / %d expected\n", matched, CALL_COUNT);
    ASSERT_EQ(matched, CALL_COUNT);

    cbm_free_result(r);
    free(src);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: Go calls at exactly 1024 (well past 512 cap)
 *
 * Larger count to ensure the fix handles 2x overflow gracefully.
 * ═══════════════════════════════════════════════════════════════════ */

TEST(go_calls_exceed_1024) {
    const int CALL_COUNT = 1024;
    size_t buf_sz = 256 + (size_t)CALL_COUNT * 40;
    char *src = malloc(buf_sz);
    ASSERT_NOT_NULL(src);

    char *p = src;
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "package main\n\nfunc main() {\n");
    for (int i = 0; i < CALL_COUNT; i++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "\tfunc_%d()\n", i);
    }
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "}\n");

    CBMFileResult *r = extract(src, CBM_LANG_GO, "test", "many_calls.go");
    ASSERT_NOT_NULL(r);
    ASSERT_FALSE(r->has_error);

    int matched = 0;
    for (int i = 0; i < r->calls.count; i++) {
        if (strncmp(r->calls.items[i].callee_name, "func_", 5) == 0) {
            matched++;
        }
    }

    printf("    calls extracted: %d / %d expected\n", matched, CALL_COUNT);
    ASSERT_EQ(matched, CALL_COUNT);

    cbm_free_result(r);
    free(src);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: Express-style route file (original reporter's scenario)
 *
 * ~150 route definitions — the actual use case from issue #199.
 * Each route has a handler call, so both defs and calls matter.
 * ═══════════════════════════════════════════════════════════════════ */

TEST(express_routes_exceed_512) {
    const int ROUTE_COUNT = 150;
    /* Each route: app.get('/route_NNN', handler_NNN); */
    size_t buf_sz = 512 + (size_t)ROUTE_COUNT * 80;
    char *src = malloc(buf_sz);
    ASSERT_NOT_NULL(src);

    char *p = src;
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "const express = require('express');\n");
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "const app = express();\n\n");
    for (int i = 0; i < ROUTE_COUNT; i++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)),
                      "app.get('/route_%d', handler_%d);\n", i, i);
    }
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "\napp.listen(3000);\n");

    CBMFileResult *r = extract(src, CBM_LANG_JAVASCRIPT, "test", "routes.js");
    ASSERT_NOT_NULL(r);
    ASSERT_FALSE(r->has_error);

    /* Each app.get() is a call — count calls containing "get" */
    int get_calls = 0;
    for (int i = 0; i < r->calls.count; i++) {
        if (strstr(r->calls.items[i].callee_name, "get") != NULL) {
            get_calls++;
        }
    }

    printf("    route calls extracted: %d / %d expected\n", get_calls, ROUTE_COUNT);
    ASSERT_EQ(get_calls, ROUTE_COUNT);

    cbm_free_result(r);
    free(src);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: ES imports exceeding 512 (walk_es_imports uses same cap)
 *
 * Generate a TS file with 600 import statements.
 * ═══════════════════════════════════════════════════════════════════ */

TEST(ts_imports_exceed_512) {
    const int IMPORT_COUNT = 600;
    size_t buf_sz = 256 + (size_t)IMPORT_COUNT * 64;
    char *src = malloc(buf_sz);
    ASSERT_NOT_NULL(src);

    char *p = src;
    for (int i = 0; i < IMPORT_COUNT; i++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)),
                      "import { mod_%d } from './module_%d';\n", i, i);
    }
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "console.log('done');\n");

    CBMFileResult *r = extract(src, CBM_LANG_TYPESCRIPT, "test", "many_imports.ts");
    ASSERT_NOT_NULL(r);
    ASSERT_FALSE(r->has_error);

    printf("    imports extracted: %d / %d expected\n", r->imports.count, IMPORT_COUNT);
    ASSERT_GTE(r->imports.count, IMPORT_COUNT);

    cbm_free_result(r);
    free(src);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: Deeply nested calls (tests stack depth, not just breadth)
 *
 * Generates nested function calls: a(b(c(d(e(...)))))
 * A deep call chain can overflow the stack even with fewer total nodes.
 * ═══════════════════════════════════════════════════════════════════ */

TEST(js_deeply_nested_calls) {
    const int DEPTH = 200;
    /* Build: outermost( level_0( level_1( ... level_199() ... ))) */
    size_t buf_sz = 256 + (size_t)DEPTH * 32;
    char *src = malloc(buf_sz);
    ASSERT_NOT_NULL(src);

    char *p = src;
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "function main() {\n  ");
    for (int i = 0; i < DEPTH; i++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "level_%d(", i);
    }
    /* Close all parens */
    for (int i = 0; i < DEPTH; i++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), ")");
    }
    p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), ";\n}\n");

    CBMFileResult *r = extract(src, CBM_LANG_JAVASCRIPT, "test", "nested_calls.js");
    ASSERT_NOT_NULL(r);
    ASSERT_FALSE(r->has_error);

    int matched = 0;
    for (int i = 0; i < r->calls.count; i++) {
        if (strncmp(r->calls.items[i].callee_name, "level_", 6) == 0) {
            matched++;
        }
    }

    printf("    nested calls extracted: %d / %d expected\n", matched, DEPTH);
    ASSERT_EQ(matched, DEPTH);

    cbm_free_result(r);
    free(src);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: YAML variables exceeding 256 (walk_variables_iter cap)
 *
 * Generate a YAML file with 300 top-level keys.
 * ═══════════════════════════════════════════════════════════════════ */

TEST(yaml_vars_exceed_256) {
    const int VAR_COUNT = 300;
    size_t buf_sz = (size_t)VAR_COUNT * 32;
    char *src = malloc(buf_sz);
    ASSERT_NOT_NULL(src);

    char *p = src;
    for (int i = 0; i < VAR_COUNT; i++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "key_%d: value_%d\n", i, i);
    }

    CBMFileResult *r = extract(src, CBM_LANG_YAML, "test", "many_keys.yaml");
    ASSERT_NOT_NULL(r);
    ASSERT_FALSE(r->has_error);

    int var_count = 0;
    for (int i = 0; i < r->defs.count; i++) {
        if (strcmp(r->defs.items[i].label, "Variable") == 0) {
            var_count++;
        }
    }

    printf("    YAML vars extracted: %d / %d expected\n", var_count, VAR_COUNT);
    ASSERT_EQ(var_count, VAR_COUNT);

    cbm_free_result(r);
    free(src);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: tree-sitter runtime allocator is bound to cbm's allocator (#424)
 *
 * cbm_init() must bind the vendored tree-sitter runtime to mimalloc via
 * ts_set_allocator(). Otherwise the runtime allocates through its overridable
 * ts_current_malloc/free defaults (plain malloc/free); under the production
 * MI_OVERRIDE=1 build (esp. Windows static-MinGW + --allow-multiple-definition)
 * ts_malloc and ts_free can resolve to different allocators, corrupting the
 * heap and crashing mid-parse on large templated C++ headers. Asserting the
 * binding reproduces the mismatch CONDITION deterministically on every
 * platform: RED before the fix (ts_current_free == plain free), GREEN after.
 * ═══════════════════════════════════════════════════════════════════ */

TEST(ts_allocator_bound_to_mimalloc_issue424) {
    cbm_init();
#if defined(CBM_BIND_TS_ALLOCATOR) && CBM_BIND_TS_ALLOCATOR
    /* Production build: cbm_init must have bound the ts runtime to mimalloc so
     * ts_malloc and ts_free can never resolve to different allocators (#424). */
    ASSERT_TRUE(ts_current_malloc == mi_malloc);
    ASSERT_TRUE(ts_current_free == mi_free);
#else
    /* Test build (MI_OVERRIDE=0, CRT + ASan): binding is intentionally OFF (it
     * would mismatch ASan/CRT frees). The crash only affects the prod binary;
     * its reproduction runs in the prod smoke job. Independently verify the fix
     * MECHANISM — ts_set_allocator wires the runtime hooks — then restore the
     * CRT defaults so the rest of this build stays allocator-consistent. */
    void *(*save_m)(size_t) = ts_current_malloc;
    void *(*save_c)(size_t, size_t) = ts_current_calloc;
    void *(*save_r)(void *, size_t) = ts_current_realloc;
    void (*save_f)(void *) = ts_current_free;
    ts_set_allocator(mi_malloc, mi_calloc, mi_realloc, mi_free);
    int wired = (ts_current_malloc == mi_malloc) && (ts_current_free == mi_free);
    ts_set_allocator(save_m, save_c, save_r, save_f); /* restore BEFORE asserting */
    ASSERT_TRUE(wired);
    ASSERT_TRUE(ts_current_free == save_f);
#endif
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Test: large templated C++ header extraction does not crash (#424)
 *
 * Generates a json.hpp-style header (hundreds of templated structs with
 * operator overloads) — the parse path that SEGV'd in #424. Asserts
 * extraction completes. (In the MI_OVERRIDE=0 test build the allocator is
 * consistent so this cannot fault here; it guards extraction on large
 * templated input and addresses the C++ gap in this file. The production-
 * binary crash reproduction runs in the smoke job.)
 * ═══════════════════════════════════════════════════════════════════ */

TEST(cpp_large_templated_header_no_crash_issue424) {
    const int N = 400; /* ~400 templated structs → ~2400 lines */
    size_t buf_sz = (size_t)N * 600;
    char *src = malloc(buf_sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, buf_sz, "#include <cstddef>\nnamespace ns {\n");
    for (int i = 0; i < N; i++) {
        p += snprintf(p, (size_t)(buf_sz - (size_t)(p - src)),
                      "template <typename T> struct Box%d {\n"
                      "  T value;\n"
                      "  bool operator<(const Box%d &o) const { return value < o.value; }\n"
                      "  bool operator==(const Box%d &o) const { return value == o.value; }\n"
                      "  T get() const { return value; }\n"
                      "};\n",
                      i, i, i);
    }
    snprintf(p, (size_t)(buf_sz - (size_t)(p - src)), "}\n");

    CBMFileResult *r = extract(src, CBM_LANG_CPP, "test", "templated.hpp");
    ASSERT_NOT_NULL(r); /* no crash is the real assertion */
    cbm_free_result(r);
    free(src);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * LSP resolve recursion guards (perf-sweep crashes, 2026-06-10)
 *
 * Indexing real OSS crashed in the LSP RESOLVE walks (distinct from the
 * extraction-walk caps above): elasticsearch → SIGSEGV under deep recursive
 * java_resolve_calls_in_node frames (bind_lambda_args), bitcoin → SIGSEGV
 * under deep c_resolve_calls_in_node frames (cbm_type_substitute via
 * c_adl_resolve), microsoft/TypeScript → SIGBUS under an unbounded
 * lookup_member_type cycle. The walks now carry depth guards; these
 * reproductions fork a child so a regression cannot kill the test runner
 * (the TS cyclic-type shape is only reachable with a real cross-file
 * registry, so that one is verified at the real-repo tier; the synthetic
 * cyclic fixture here guards the in-file path).
 * ═══════════════════════════════════════════════════════════════════ */

#if !defined(_WIN32)
#include <sys/wait.h>
#include <unistd.h>
#endif

/* Run cbm_extract_file in a forked child; true if the child died by signal.
 * Mirrors tests/test_lang_contract.c. On Windows run in-process (a genuine
 * crash there aborts the runner — hard, visible failure). */
static bool so_extract_crashes(const char *content, CBMLanguage lang, const char *relpath) {
#if defined(_WIN32)
    CBMFileResult *r =
        cbm_extract_file(content, (int)strlen(content), lang, "so", relpath, 0, NULL, NULL);
    if (r) {
        cbm_free_result(r);
    }
    return false;
#else
    fflush(NULL);
    pid_t pid = fork();
    if (pid < 0) {
        return false;
    }
    if (pid == 0) {
        CBMFileResult *r =
            cbm_extract_file(content, (int)strlen(content), lang, "so", relpath, 0, NULL, NULL);
        if (r) {
            cbm_free_result(r);
        }
        _exit(0);
    }
    int status = 0;
    (void)waitpid(pid, &status, 0);
    return WIFSIGNALED(status);
#endif
}

#if !defined(_WIN32)
/* Child-side alarm handler: a clean exit on the deadline — never reached on
 * a crash (SIGSEGV/SIGBUS terminate the child first and stay visible). */
static void so_parse_alarm_exit(int sig) {
    (void)sig;
    _exit(0);
}

#endif

/* Parse `content` with tree-sitter DIRECTLY in a forked child — bypassing
 * cbm_extract_file's Perl pre-parse nesting guard — returning true if the child
 * died by signal. This is the crash-isolating regression for the vendored GLR
 * stack-merge recursion cap (CBM_TS_STACK_MERGE_MAX_DEPTH, ts_runtime/src/stack.c):
 * the extract-level guard skips pathologically nested Perl before it reaches the
 * parser, so only a direct parse exercises the cap. Windows runs in-process (a
 * real crash aborts the runner — a visible failure), mirroring so_extract_crashes. */
static bool so_parse_crashes(const char *content, CBMLanguage lang) {
    const TSLanguage *ts_lang = cbm_ts_language(lang);
    if (!ts_lang) {
        return false;
    }
#if defined(_WIN32)
    TSParser *parser = ts_parser_new();
    if (parser) {
        ts_parser_set_language(parser, ts_lang);
        TSTree *tree = ts_parser_parse_string(parser, NULL, content, (uint32_t)strlen(content));
        if (tree) {
            ts_tree_delete(tree);
        }
        ts_parser_delete(parser);
    }
    return false;
#else
    fflush(NULL);
    pid_t pid = fork();
    if (pid < 0) {
        return false;
    }
    if (pid == 0) {
        /* Deterministic ceiling. Falsification data (2026-07-18, macOS
         * ASan): with CBM_TS_STACK_MERGE_MAX_DEPTH deleted outright, this
         * branch stayed GREEN both unbounded (220s full grind, no crash)
         * and bounded — instrumentation shows the recursive merge path is
         * never even entered here (max depth 0 through the whole window).
         * The cap-regression DETECTION therefore lives in the Windows
         * in-process branch above (real ~1 MB native stack, where #913
         * manifested); this POSIX branch is a bounded no-crash smoke of
         * the pathological parse, and grinding past 10s adds no signal
         * (unbounded it wandered 2s-218s with machine state). A real
         * SIGSEGV/SIGBUS still terminates the child by signal before the
         * alarm handler can exit cleanly. */
        signal(SIGALRM, so_parse_alarm_exit);
        alarm(10);
        TSParser *parser = ts_parser_new();
        if (parser) {
            ts_parser_set_language(parser, ts_lang);
            TSTree *tree = ts_parser_parse_string(parser, NULL, content, (uint32_t)strlen(content));
            if (tree) {
                ts_tree_delete(tree);
            }
            ts_parser_delete(parser);
        }
        _exit(0);
    }
    int status = 0;
    (void)waitpid(pid, &status, 0);
    return WIFSIGNALED(status);
#endif
}

TEST(lsp_java_deep_nesting_no_crash) {
    /* Deeply nested call expressions — the same shape as the elasticsearch
     * crash (fast SIGSEGV under recursive java_resolve_calls_in_node frames;
     * pre-guard prod probe: rc=139 in under a second at this depth). Nested
     * BLOCKS are deliberately not used: java block processing is minutes-slow
     * at depth >=3000 (separate adversarial-input pathology, documented in
     * the perf-sweep report). */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 3 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "class X { static int f(int a) { return a; } static int g() { return ");
    for (int i = 0; i < DEPTH; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), "; } }\n");
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_JAVA, "X.java"));
    free(src);
    PASS();
}

TEST(lsp_cpp_deep_expression_no_crash) {
    /* See lsp_java_deep_nesting_no_crash on the depth choice. */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 3 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "int f(int x) { return x; }\nint g() { return ");
    for (int i = 0; i < DEPTH; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), "; }\n");
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_CPP, "deep.cpp"));
    free(src);
    PASS();
}

TEST(lsp_python_deep_expression_no_crash) {
    /* Issue #720: a deeply parenthesized assignment RHS drives
     * py_eval_expr_type into one native recursion frame per paren level
     * via py_process_statement's RHS inference — a path NOT covered by
     * the py_resolve_calls_in walk_depth cap (that guards the emit walk,
     * not the binder's expression evaluation). Pre-guard this SIGSEGVs.
     * PY_LSP_MAX_EVAL_DEPTH turns it into a graceful unknown. Depth
     * mirrors lsp_java_deep_nesting_no_crash. */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 2 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "def main():\n    value = ");
    memset(p, '(', DEPTH);
    p += DEPTH;
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), "\n    return value\n");
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_PYTHON, "deep_expr.py"));
    free(src);
    PASS();
}

TEST(lsp_perl_deep_expression_no_crash) {
    /* Deeply nested Perl call expressions f(f(f(...f(1)...))). Unlike the
     * Java/C++ cases, the overflow here is NOT in the LSP walk — it is in
     * tree-sitter's own GLR parser: stack_node_add_link (vendored
     * ts_runtime/src/stack.c) recurses once per nesting level while merging the
     * ambiguous parse-stack heads that Perl's `f(...)` grammar produces, blowing
     * a small (1 MB Windows) stack during the parse, before any LSP walk runs.
     * The CBM_PERL_MAX_PARSE_NESTING pre-parse guard in cbm_extract_file skips
     * such input so it never reaches tree-sitter. See
     * lsp_java_deep_nesting_no_crash on the depth choice. */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 3 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "sub f { return $_[0]; }\nsub g { return ");
    for (int i = 0; i < DEPTH; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), "; }\n");
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_PERL, "deep.pl"));
    free(src);
    PASS();
}

TEST(perl_glr_deep_parse_recursion_capped) {
    /* Issue #913 — the proper fix for what lsp_perl_deep_expression_no_crash's
     * pre-parse guard only works around. Parse deeply nested ambiguous Perl
     * f(f(f(...f(1)...))) DIRECTLY (past the CBM_PERL_MAX_PARSE_NESTING guard,
     * which would otherwise skip it). Perl's paren-optional call grammar makes
     * each level ambiguous, so tree-sitter's GLR parser merges the ambiguous
     * parse-stack heads recursively — stack_node_add_link in
     * ts_runtime/src/stack.c, once per nesting level — overflowing the native
     * stack (a ~1 MB Windows stack, and even an 8 MB POSIX stack at this depth)
     * during the parse, before any extraction runs. The
     * CBM_TS_STACK_MERGE_MAX_DEPTH cap stops merging past the bound: the
     * ambiguity is left on the GLR stack instead of merged — a valid parse,
     * never a wrong one — so the parse returns cleanly instead of crashing.
     * Depth mirrors lsp_perl_deep_expression_no_crash. */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 3 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "sub f { return $_[0]; }\nsub g { return ");
    for (int i = 0; i < DEPTH; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), "; }\n");
    ASSERT_FALSE(so_parse_crashes(src, CBM_LANG_PERL));
    free(src);
    PASS();
}

TEST(lsp_java_lambda_args_exceed_params_no_crash) {
    /* A call with MORE arguments than the resolved method's declared params:
     * bind_lambda_args indexed the NULL-terminated signature param_types array
     * by the call-site argument index, reading past the terminator — a garbage
     * CBMType* then got dereferenced (elasticsearch SIGSEGV, java_lsp.c:2364
     * via :2722; same OOB family as #427). */
    const char *src = "class A {\n"
                      "    void run(Runnable r) {}\n"
                      "    void go() {\n"
                      "        run(() -> {}, () -> {}, () -> {}, () -> {}, () -> {}, () -> {});\n"
                      "    }\n"
                      "}\n";
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_JAVA, "A.java"));
    PASS();
}

TEST(lsp_ts_cyclic_types_no_crash) {
    const char *src = "type A = B | null;\n"
                      "type B = A | number;\n"
                      "interface C extends D { c: number; }\n"
                      "interface D extends C { d: number; }\n"
                      "declare const a: A;\n"
                      "declare const c: C;\n"
                      "function useIt(p: C) { return p.missing_member; }\n"
                      "const y = c.also_missing;\n";
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_TYPESCRIPT, "cycle.ts"));
    PASS();
}

/* ─── Deeply-nested calls drive the per-language LSP resolve walkers into
 * per-nesting-level native recursion. Unguarded, these SIGSEGV and take down
 * the whole index. Each walker now has a walk_depth cap (CBM_LSP_MAX_WALK_DEPTH,
 * env-overridable) that skips the too-deep subtree — graceful degradation.
 * These mirror lsp_java_deep_nesting_no_crash: same fixture shape, one per
 * previously-unguarded walker (py_resolve_calls_in, resolve_calls_in_node[go],
 * php_resolve_calls_in_node, kt_resolve_calls_in_node). RED proof: run the
 * suite with CBM_LSP_MAX_WALK_DEPTH set huge (disabling only these caps) and
 * each of these four SIGSEGVs — proving the guard, not the fixture, is what
 * keeps them green. ─── */

TEST(lsp_python_deep_nesting_no_crash) {
    /* py_resolve_calls_in recurses per nesting level; see the Java analog. */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 3 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "def f(a):\n    return a\ndef g():\n    return ");
    for (int i = 0; i < DEPTH; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), "\n");
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_PYTHON, "deep.py"));
    free(src);
    PASS();
}

TEST(lsp_go_deep_nesting_no_crash) {
    /* resolve_calls_in_node recurses per nesting level; see the Java analog. */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 3 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "package p\nfunc f(a int) int { return a }\nfunc g() int { return ");
    for (int i = 0; i < DEPTH; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), " }\n");
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_GO, "deep.go"));
    free(src);
    PASS();
}

TEST(lsp_php_deep_nesting_no_crash) {
    /* php_resolve_calls_in_node recurses per nesting level; Java analog. */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 3 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "<?php\nfunction f($a) { return $a; }\nfunction g() { return ");
    for (int i = 0; i < DEPTH; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), "; }\n");
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_PHP, "deep.php"));
    free(src);
    PASS();
}

TEST(lsp_kotlin_deep_nesting_no_crash) {
    /* kt_resolve_calls_in_node recurses per nesting level; Java analog. */
    const int DEPTH = 30000;
    size_t sz = (size_t)DEPTH * 3 + 256;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    p += snprintf(p, sz, "fun f(a: Int): Int { return a }\nfun g(): Int { return ");
    for (int i = 0; i < DEPTH; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', DEPTH);
    p += DEPTH;
    snprintf(p, sz - (size_t)(p - src), " }\n");
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_KOTLIN, "deep.kt"));
    free(src);
    PASS();
}

/* ── Batch B: pathological-shape hangs and string-literal fidelity ─────────── */

/* Extract `src` and return elapsed milliseconds (the result is freed). */
static uint64_t so_timed_extract(const char *src, CBMLanguage lang, const char *path) {
    uint64_t t0 = cbm_now_ms();
    CBMFileResult *r = cbm_extract_file(src, (int)strlen(src), lang, "so", path, 0, NULL, NULL);
    uint64_t dt = cbm_now_ms() - t0;
    if (r) {
        cbm_free_result(r);
    }
    return dt;
}

/* Return the first_string_arg of the first call whose callee contains `needle`. */
static const char *so_first_string_arg(CBMFileResult *r, const char *needle) {
    for (int i = 0; i < r->calls.count; i++) {
        const char *callee = r->calls.items[i].callee_name;
        if (callee && strstr(callee, needle)) {
            return r->calls.items[i].first_string_arg;
        }
    }
    return NULL;
}

/* B1: Clojure `(defn g [] (f (f ... x)))` -- every call list climbed all of its
 * ancestors with ts_node_parent (itself O(depth)), so deep nesting was cubic. */
TEST(clojure_deep_nested_calls_no_hang) {
    enum { DEPTH = 5000 };
    size_t sz = (size_t)DEPTH * 6 + 64;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src + snprintf(src, sz, "(defn g [] ");
    for (int i = 0; i < DEPTH; i++) {
        p += snprintf(p, sz - (size_t)(p - src), "(f ");
    }
    p += snprintf(p, sz - (size_t)(p - src), "x");
    memset(p, ')', (size_t)DEPTH + 1);
    p[DEPTH + 1] = '\0';
    uint64_t ms = so_timed_extract(src, CBM_LANG_CLOJURE, "deep.clj");
    free(src);
    ASSERT_LT(ms, 20000);
    PASS();
}

/* B2: JS `a.m().m()...` -- the callee text of each member call was the whole
 * receiver chain, O(N^2) memory and time. The chain is sized so the remaining
 * (scope-stack) cost stays small; the callee-length check is what separates
 * the fixed extractor from the unfixed one. */
TEST(js_long_member_chain_no_hang) {
    enum { CHAIN = 10000 };
    size_t sz = (size_t)CHAIN * 4 + 16;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    *p++ = 'a';
    for (int i = 0; i < CHAIN; i++) {
        memcpy(p, ".m()", 4);
        p += 4;
    }
    *p++ = ';';
    *p = '\0';
    uint64_t t0 = cbm_now_ms();
    CBMFileResult *r =
        cbm_extract_file(src, (int)strlen(src), CBM_LANG_JAVASCRIPT, "so", "chain.js", 0, NULL, NULL);
    uint64_t ms = cbm_now_ms() - t0;
    free(src);
    ASSERT_NOT_NULL(r);
    ASSERT_GT(r->calls.count, 0);
    /* No callee name may carry the receiver chain: that is the O(N^2) text. */
    size_t longest = 0;
    for (int i = 0; i < r->calls.count; i++) {
        const char *callee = r->calls.items[i].callee_name;
        size_t len = callee ? strlen(callee) : 0;
        if (len > longest) {
            longest = len;
        }
    }
    cbm_free_result(r);
    ASSERT_LT(longest, 1024);
    ASSERT_LT(ms, 20000);
    PASS();
}

/* B3: raw / prefixed / triple-quoted string literals yield the bare path. */
TEST(go_raw_string_route_path_stripped) {
    const char *src = "package main\n"
                      "func main() {\n"
                      "\tr.GET(`/users`, h)\n"
                      "\tr.POST(\"/dq\", h)\n"
                      "}\n";
    CBMFileResult *r = extract(src, CBM_LANG_GO, "so", "routes.go");
    ASSERT_NOT_NULL(r);
    const char *raw = so_first_string_arg(r, "GET");
    ASSERT_NOT_NULL(raw);
    ASSERT_STR_EQ("/users", raw);
    const char *dq = so_first_string_arg(r, "POST");
    ASSERT_NOT_NULL(dq);
    ASSERT_STR_EQ("/dq", dq);
    cbm_free_result(r);
    PASS();
}

TEST(python_prefixed_string_route_path_stripped) {
    const char *src = "import requests\n"
                      "def run():\n"
                      "    requests.get(r\"/raw\")\n"
                      "    requests.post(f\"/fmt\")\n"
                      "    requests.put(\"\"\"/tri\"\"\")\n"
                      "    requests.delete(b'/byt')\n";
    CBMFileResult *r = extract(src, CBM_LANG_PYTHON, "so", "routes.py");
    ASSERT_NOT_NULL(r);
    const char *a = so_first_string_arg(r, "get");
    ASSERT_NOT_NULL(a);
    ASSERT_STR_EQ("/raw", a);
    const char *b = so_first_string_arg(r, "post");
    ASSERT_NOT_NULL(b);
    ASSERT_STR_EQ("/fmt", b);
    const char *c = so_first_string_arg(r, "put");
    ASSERT_NOT_NULL(c);
    ASSERT_STR_EQ("/tri", c);
    const char *d = so_first_string_arg(r, "delete");
    ASSERT_NOT_NULL(d);
    ASSERT_STR_EQ("/byt", d);
    cbm_free_result(r);
    PASS();
}

/* B5: `socket.emit("user:" + id, d)` must not record the channel "user:". */
TEST(js_emit_concat_channel_not_truncated) {
    const char *src = "function run(id, d) {\n"
                      "  socket.emit(\"user:\" + id, d);\n"
                      "  socket.emit(\"ok\", d);\n"
                      "}\n";
    CBMFileResult *r = extract(src, CBM_LANG_JAVASCRIPT, "so", "emit.js");
    ASSERT_NOT_NULL(r);
    bool saw_ok = false;
    for (int i = 0; i < r->channels.count; i++) {
        ASSERT_STR_NEQ("user:", r->channels.items[i].channel_name);
        if (strcmp(r->channels.items[i].channel_name, "ok") == 0) {
            saw_ok = true;
        }
    }
    ASSERT_TRUE(saw_ok);
    cbm_free_result(r);
    PASS();
}

/* B6: many bindings of one name followed by many usages scanned every binding
 * per usage. */
TEST(go_many_shadow_bindings_no_slowdown) {
    enum { N = 40000 };
    size_t sz = (size_t)N * 24 + 64;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src + snprintf(src, sz, "package main\nfunc g() {\n");
    for (int i = 0; i < N; i++) {
        p += snprintf(p, sz - (size_t)(p - src), "\t_ = x\n");
    }
    for (int i = 0; i < N; i++) {
        p += snprintf(p, sz - (size_t)(p - src), "\tx := 1\n");
    }
    snprintf(p, sz - (size_t)(p - src), "}\n");
    uint64_t ms = so_timed_extract(src, CBM_LANG_GO, "shadow.go");
    free(src);
    ASSERT_LT(ms, 20000);
    PASS();
}

/* ═══════════════════════════════════════════════════════════════════
 * Suite registration
 * ═══════════════════════════════════════════════════════════════════ */

/* Split into three sub-suites so parallel/sharded runs are not serialized
 * behind one ~4-minute suite (it was the wall-clock critical path: every
 * other suite finished underneath it). Pure re-registration — the 20
 * RUN_TEST entries are exactly the ones the single suite carried; the
 * before/after test-count parity is asserted in the shard runner. */
/* The dbt Jinja walk recursed once per syntax level with no cap; deeply nested calls
 * overflowed the stack. Same depth as the other *_deep_* cases. */
static char *so_nested_calls(const char *prefix, const char *suffix, int depth) {
    size_t sz = strlen(prefix) + strlen(suffix) + (size_t)depth * 3 + 16;
    char *src = malloc(sz);
    if (!src) {
        return NULL;
    }
    char *p = src + snprintf(src, sz, "%s", prefix);
    for (int i = 0; i < depth; i++) {
        *p++ = 'f';
        *p++ = '(';
    }
    *p++ = '1';
    memset(p, ')', (size_t)depth);
    p += depth;
    snprintf(p, sz - (size_t)(p - src), "%s", suffix);
    return src;
}

TEST(dbt_deep_jinja_nesting_no_crash) {
    char *src = so_nested_calls("select 1 from {{ ref('a') }} {{ ", " }}\n", 30000);
    ASSERT_NOT_NULL(src);
    ASSERT_FALSE(so_extract_crashes(src, CBM_LANG_SQL, "models/deep.sql"));
    free(src);
    PASS();
}

/* ── Batch U: scope-stack, binding-index and infra-scan costs ─────────────── */

/* U1: every node of a 30000-link `a.m().m()...` chain consulted the whole walk
 * scope stack (existing lexical scope, innermost lexical scope, enclosing
 * function), so the walk was quadratic in the nesting depth. */
TEST(js_member_chain_30000_scope_stack_linear) {
    enum { CHAIN = 30000 };
    size_t sz = (size_t)CHAIN * 4 + 16;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    *p++ = 'a';
    for (int i = 0; i < CHAIN; i++) {
        memcpy(p, ".m()", 4);
        p += 4;
    }
    *p++ = ';';
    *p = '\0';
    uint64_t ms = so_timed_extract(src, CBM_LANG_JAVASCRIPT, "chain30k.js");
    free(src);
    printf("    30000-link chain: %llu ms\n", (unsigned long long)ms);
    ASSERT_LT(ms, 20000);
    PASS();
}

/* U2: cbm_scope_bind_value scanned every chunk with strcmp, so N unique names
 * in one scope cost O(N^2). */
TEST(python_many_unique_names_one_scope_linear) {
    enum { N = 60000 };
    size_t sz = (size_t)N * 16 + 16;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    for (int i = 0; i < N; i++) {
        p += snprintf(p, sz - (size_t)(p - src), "x%d = 1\n", i);
    }
    uint64_t ms = so_timed_extract(src, CBM_LANG_PYTHON, "names.py");
    free(src);
    printf("    %d unique top-level names: %llu ms\n", (int)N, (unsigned long long)ms);
    ASSERT_LT(ms, 20000);
    PASS();
}

/* U3a: the infra scan re-walked the whole subtree at every array, so a deeply
 * nested JSON `[[[...]]]` was quadratic. */
TEST(json_deep_array_infra_scan_linear) {
    enum { DEPTH = 30000 };
    size_t sz = (size_t)DEPTH * 2 + 16;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    memset(src, '[', (size_t)DEPTH);
    memset(src + DEPTH, ']', (size_t)DEPTH);
    src[2 * DEPTH] = '\n';
    src[2 * DEPTH + 1] = '\0';
    uint64_t ms = so_timed_extract(src, CBM_LANG_JSON, "deep.json");
    free(src);
    printf("    JSON %d-deep array: %llu ms\n", (int)DEPTH, (unsigned long long)ms);
    ASSERT_LT(ms, 20000);
    PASS();
}

static bool so_has_infra_binding(const CBMFileResult *r, const char *source, const char *url) {
    for (int i = 0; i < r->infra_bindings.count; i++) {
        const CBMInfraBinding *b = &r->infra_bindings.items[i];
        if (b->source_name && b->target_url && strcmp(b->source_name, source) == 0 &&
            strcmp(b->target_url, url) == 0) {
            return true;
        }
    }
    return false;
}

/* U3b: the infra scanner stopped pushing at a fixed 512-frame stack, so a YAML
 * sequence with more than 512 items lost items. Every item must be bound, and
 * each exactly once (the old scan emitted a binding per enclosing container). */
TEST(yaml_infra_sequence_items_exceed_512) {
    enum { ITEMS = 700 };
    size_t sz = (size_t)ITEMS * 96 + 64;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    for (int i = 0; i < ITEMS; i++) {
        p += snprintf(p, sz - (size_t)(p - src),
                      "- topic: t%d\n  uri: https://svc%d.example.com/hook\n", i, i);
    }
    CBMFileResult *r = extract(src, CBM_LANG_YAML, "so", "pubsub.yaml");
    free(src);
    ASSERT_NOT_NULL(r);
    bool first = so_has_infra_binding(r, "t0", "https://svc0.example.com/hook");
    bool last = so_has_infra_binding(r, "t699", "https://svc699.example.com/hook");
    int total = r->infra_bindings.count;
    cbm_free_result(r);
    ASSERT_TRUE(first);
    ASSERT_TRUE(last);
    ASSERT_EQ(total, ITEMS);
    PASS();
}

/* U3c: push_yaml_block_children stopped pushing at 256 frames, so a mapping
 * with more than 256 nested-map keys dropped the later keys' leaf values. */
TEST(yaml_nested_map_keys_exceed_256) {
    enum { KEYS = 400 };
    size_t sz = (size_t)KEYS * 80 + 64;
    char *src = malloc(sz);
    ASSERT_NOT_NULL(src);
    char *p = src;
    for (int i = 0; i < KEYS; i++) {
        p += snprintf(p, sz - (size_t)(p - src), "k%d:\n  endpoint: https://svc%d.example.com/x\n", i,
                      i);
    }
    CBMFileResult *r = extract(src, CBM_LANG_YAML, "so", "many_maps.yaml");
    free(src);
    ASSERT_NOT_NULL(r);
    bool saw_last = false;
    bool saw_first = false;
    for (int i = 0; i < r->string_refs.count; i++) {
        const char *kp = r->string_refs.items[i].key_path;
        if (kp && strcmp(kp, "k399.endpoint") == 0) {
            saw_last = true;
        }
        if (kp && strcmp(kp, "k0.endpoint") == 0) {
            saw_first = true;
        }
    }
    cbm_free_result(r);
    ASSERT_TRUE(saw_first);
    ASSERT_TRUE(saw_last);
    PASS();
}

/* ── Batch V: superlinear usage classification and YAML nested walk ───────── */

/* Build `<head>` + `<open>` x depth + `<core>` + `<close>` x depth + `<tail>`. */
static char *so_nested_wrap(const char *head, const char *open, int depth, const char *core,
                            const char *close, const char *tail) {
    size_t sz = strlen(head) + strlen(core) + strlen(tail) +
                (size_t)depth * (strlen(open) + strlen(close)) + 16;
    char *src = malloc(sz);
    if (!src) {
        return NULL;
    }
    char *p = src + snprintf(src, sz, "%s", head);
    for (int i = 0; i < depth; i++) {
        p += snprintf(p, sz - (size_t)(p - src), "%s", open);
    }
    p += snprintf(p, sz - (size_t)(p - src), "%s", core);
    for (int i = 0; i < depth; i++) {
        p += snprintf(p, sz - (size_t)(p - src), "%s", close);
    }
    snprintf(p, sz - (size_t)(p - src), "%s", tail);
    return src;
}

/* V1a: a C++ initializer list nested 8000 deep, `int a[] = {x, {x, ...}};`.
 * Every `x` was classified by climbing all of its ancestors (a cursor reset plus
 * one hop per level, ts_node_parent being O(depth) itself), so handle_usages was
 * superlinear: 57 s at this depth before the fix. The deep `std::vector<`
 * shape from the same report is covered by the work counter test below -- its
 * wall time is dominated by the vendored tree-sitter GLR parse, not by usages. */
TEST(cpp_deep_initializer_list_usages_linear) {
    char *src = so_nested_wrap("int a[] = ", "{x, ", 8000, "x", "}", ";\n");
    ASSERT_NOT_NULL(src);
    uint64_t ms = so_timed_extract(src, CBM_LANG_CPP, "deep_init.cpp");
    free(src);
    printf("    C++ 8000-deep {x, ...}: %llu ms\n", (unsigned long long)ms);
    ASSERT_LT(ms, 20000);
    PASS();
}

#if defined(CBM_CALL_REFERENCE_LOOKUP_TEST_API) && CBM_CALL_REFERENCE_LOOKUP_TEST_API
static uint64_t so_cpp_template_climb_work(int depth) {
    char *src = so_nested_wrap("using T = ", "std::vector<", depth, "int", "> ", ";\n");
    if (!src) {
        return UINT64_MAX;
    }
    cbm_usage_field_lookup_test_reset();
    CBMFileResult *r =
        cbm_extract_file(src, (int)strlen(src), CBM_LANG_CPP, "so", "deep_tmpl.cpp", 0, NULL, NULL);
    uint64_t work = cbm_usage_field_lookup_test_work();
    free(src);
    if (!r) {
        return UINT64_MAX;
    }
    cbm_free_result(r);
    return work;
}
#endif

/* V1a (work form): the climb steps taken while classifying `std::vector<...>`
 * type references must grow with the input, not with its square. A 4x deeper
 * input costs at most 8x the steps (16x when quadratic). Counts cursor hops, so
 * it does not depend on the machine or on the vendored parser's speed. */
TEST(cpp_deep_template_args_usage_work_linear) {
#if defined(CBM_CALL_REFERENCE_LOOKUP_TEST_API) && CBM_CALL_REFERENCE_LOOKUP_TEST_API
    uint64_t small = so_cpp_template_climb_work(400);
    uint64_t big = so_cpp_template_climb_work(1600);
    ASSERT_TRUE(small != UINT64_MAX);
    ASSERT_TRUE(big != UINT64_MAX);
    printf("    std::vector< climb work: depth 400 = %llu, depth 1600 = %llu\n",
           (unsigned long long)small, (unsigned long long)big);
    ASSERT_LT(big, small * 8 + 256);
    PASS();
#else
    SKIP("climb work counter needs CBM_CALL_REFERENCE_LOOKUP_TEST_API");
#endif
}

/* V1b: a Java field `List<List<...>>` nested 4000 deep (16-18 s at 3200). */
TEST(java_deep_generics_usages_linear) {
    char *src = so_nested_wrap("class A {\n  ", "List<", 4000, "String", ">", " f;\n}\n");
    ASSERT_NOT_NULL(src);
    uint64_t ms = so_timed_extract(src, CBM_LANG_JAVA, "A.java");
    free(src);
    printf("    Java 4000-deep List<: %llu ms\n", (unsigned long long)ms);
    ASSERT_LT(ms, 20000);
    PASS();
}

/* V1c: a TypeScript `let x: Array<Array<...>>` nested 4000 deep (31.8 s). */
TEST(ts_deep_generics_usages_linear) {
    char *src = so_nested_wrap("let x: ", "Array<", 4000, "number", ">", ";\n");
    ASSERT_NOT_NULL(src);
    uint64_t ms = so_timed_extract(src, CBM_LANG_TYPESCRIPT, "deep.ts");
    free(src);
    printf("    TS 4000-deep Array<: %llu ms\n", (unsigned long long)ms);
    ASSERT_LT(ms, 20000);
    PASS();
}

static int so_count_key_path(const CBMFileResult *r, const char *path) {
    int n = 0;
    for (int i = 0; i < r->string_refs.count; i++) {
        const char *kp = r->string_refs.items[i].key_path;
        if (kp && strcmp(kp, path) == 0) {
            n++;
        }
    }
    return n;
}

/* V2a: output contract of the YAML nested-key walk, small input. A leaf is
 * reported once per enclosing mapping chain suffix (a.b.url, b.url, url). */
TEST(yaml_nested_output_unchanged) {
    const char *src = "a:\n  b:\n    url: https://svc.example.com/hook\n  c: https://c.example.com/x\n"
                      "list:\n  - m:\n      n: https://n.example.com/y\n";
    CBMFileResult *r = extract(src, CBM_LANG_YAML, "so", "small.yaml");
    ASSERT_NOT_NULL(r);
    int abu = so_count_key_path(r, "a.b.url");
    int bu = so_count_key_path(r, "b.url");
    int u = so_count_key_path(r, "url");
    int ac = so_count_key_path(r, "a.c");
    int c = so_count_key_path(r, "c");
    int mn = so_count_key_path(r, "m.n");
    int n = so_count_key_path(r, "n");
    int total = r->string_refs.count;
    cbm_free_result(r);
    ASSERT_EQ(abu, 1);
    ASSERT_EQ(bu, 1);
    ASSERT_EQ(u, 1);
    ASSERT_EQ(ac, 1);
    ASSERT_EQ(c, 1);
    ASSERT_EQ(mn, 1);
    ASSERT_EQ(n, 1);
    ASSERT_EQ(total, 7);
    PASS();
}

/* Build `depth` nested `a:` mappings (1-space steps) over `wide` scalar keys and
 * one URL leaf; extract it and return elapsed ms and the string_ref count. */
static uint64_t so_yaml_chain_extract(int depth, int wide, int *refs_out) {
    size_t sz = (size_t)depth * ((size_t)depth + 4) / 2 + (size_t)wide * ((size_t)depth + 16) + 256;
    char *src = malloc(sz);
    if (!src) {
        return 0;
    }
    char *p = src;
    for (int i = 0; i < depth; i++) {
        memset(p, ' ', (size_t)i);
        p += i;
        *p++ = 'a';
        *p++ = ':';
        *p++ = '\n';
    }
    for (int w = 0; w < wide; w++) {
        memset(p, ' ', (size_t)depth);
        p += depth;
        p += snprintf(p, sz - (size_t)(p - src), "x%d: 1\n", w);
    }
    memset(p, ' ', (size_t)depth);
    p += depth;
    snprintf(p, sz - (size_t)(p - src), "url: https://svc.example.com/hook\n");
    uint64_t t0 = cbm_now_ms();
    CBMFileResult *r = extract(src, CBM_LANG_YAML, "so", "chain.yaml");
    uint64_t ms = cbm_now_ms() - t0;
    free(src);
    *refs_out = -1;
    if (r) {
        *refs_out = r->string_refs.count;
        cbm_free_result(r);
    }
    return ms;
}

/* V2b: handle_yaml_nested re-walked the whole subtree for every nested
 * block_mapping, so each of the DEPTH ancestors of a wide mapping re-visited all
 * of its keys (and rebuilt every dotted path): quadratic in visits and in arena
 * bytes. The YAML scanner caps block nesting near 250 levels, so the shape is a
 * deep chain over a wide bottom mapping. Cost must not scale with the depth:
 * compare against the same keys under a single parent. */
TEST(yaml_deep_nested_mappings_linear) {
    enum { DEPTH = 200, WIDE = 20000 };
    int flat_refs = 0;
    int deep_refs = 0;
    uint64_t flat_ms = so_yaml_chain_extract(1, WIDE, &flat_refs);
    uint64_t deep_ms = so_yaml_chain_extract(DEPTH, WIDE, &deep_refs);
    printf("    YAML %d keys: flat %llu ms, %d-deep chain %llu ms (%d refs)\n", (int)WIDE,
           (unsigned long long)flat_ms, (int)DEPTH, (unsigned long long)deep_ms, deep_refs);
    ASSERT_EQ(flat_refs, 2);
    ASSERT_EQ(deep_refs, DEPTH + 1);
    ASSERT_LT(deep_ms, 20000);
    ASSERT_LT(deep_ms, flat_ms * 8 + 500);
    PASS();
}

SUITE(stack_overflow_a) {
    cbm_init();

    RUN_TEST(ts_allocator_bound_to_mimalloc_issue424);
    RUN_TEST(cpp_large_templated_header_no_crash_issue424);
    RUN_TEST(lsp_java_deep_nesting_no_crash);
    RUN_TEST(lsp_java_lambda_args_exceed_params_no_crash);
    RUN_TEST(lsp_cpp_deep_expression_no_crash);
    RUN_TEST(lsp_python_deep_expression_no_crash);
    RUN_TEST(lsp_perl_deep_expression_no_crash);

    cbm_shutdown();
}

SUITE(stack_overflow_b) {
    cbm_init();

    RUN_TEST(dbt_deep_jinja_nesting_no_crash);
    RUN_TEST(perl_glr_deep_parse_recursion_capped);
    RUN_TEST(lsp_ts_cyclic_types_no_crash);
    RUN_TEST(lsp_python_deep_nesting_no_crash);
    RUN_TEST(lsp_go_deep_nesting_no_crash);
    RUN_TEST(lsp_php_deep_nesting_no_crash);
    RUN_TEST(lsp_kotlin_deep_nesting_no_crash);

    cbm_shutdown();
}

SUITE(stack_overflow_c) {
    cbm_init();

    RUN_TEST(js_calls_exceed_512);
    RUN_TEST(python_calls_exceed_512);
    RUN_TEST(go_calls_exceed_1024);
    RUN_TEST(express_routes_exceed_512);
    RUN_TEST(ts_imports_exceed_512);
    RUN_TEST(js_deeply_nested_calls);
    RUN_TEST(yaml_vars_exceed_256);
    RUN_TEST(clojure_deep_nested_calls_no_hang);
    RUN_TEST(js_long_member_chain_no_hang);
    RUN_TEST(go_raw_string_route_path_stripped);
    RUN_TEST(python_prefixed_string_route_path_stripped);
    RUN_TEST(js_emit_concat_channel_not_truncated);
    RUN_TEST(go_many_shadow_bindings_no_slowdown);
    RUN_TEST(js_member_chain_30000_scope_stack_linear);
    RUN_TEST(python_many_unique_names_one_scope_linear);
    RUN_TEST(json_deep_array_infra_scan_linear);
    RUN_TEST(yaml_infra_sequence_items_exceed_512);
    RUN_TEST(yaml_nested_map_keys_exceed_256);
    RUN_TEST(cpp_deep_initializer_list_usages_linear);
    RUN_TEST(cpp_deep_template_args_usage_work_linear);
    RUN_TEST(java_deep_generics_usages_linear);
    RUN_TEST(ts_deep_generics_usages_linear);
    RUN_TEST(yaml_nested_output_unchanged);
    RUN_TEST(yaml_deep_nested_mappings_linear);

    cbm_shutdown();
}

#include "scope.h"
#include <string.h>

CBMScope* cbm_scope_push(CBMArena* a, CBMScope* current) {
    CBMScope* scope = (CBMScope*)cbm_arena_alloc(a, sizeof(CBMScope));
    if (!scope) {
        return current;
    }
    memset(scope, 0, sizeof(CBMScope));
    scope->parent = current;
    scope->arena = a;
    return scope;
}

CBMScope* cbm_scope_pop(CBMScope* scope) {
    if (!scope) {
        return NULL;
    }
    return scope->parent;
}

static CBMScopeChunk* alloc_chunk(CBMScope* scope) {
    if (!scope->arena) {
        return NULL;
    }
    CBMScopeChunk* c = (CBMScopeChunk*)cbm_arena_alloc(scope->arena, sizeof(CBMScopeChunk));
    if (!c) {
        return NULL;
    }
    memset(c, 0, sizeof(CBMScopeChunk));
    c->next = scope->chunks;
    scope->chunks = c;
    return c;
}

static void scope_index_insert(const CBMVarBinding **table, int cap, const CBMVarBinding *b,
                               uint32_t hash) {
    uint32_t mask = (uint32_t)cap - 1U;
    uint32_t i = hash & mask;
    while (table[i]) {
        i = (i + 1U) & mask;
    }
    table[i] = b;
}

/* Rebuild the index at new_cap slots from the chunks. Keeps the old index (and
 * returns false) when the allocation fails. */
static bool scope_index_rebuild(CBMScope *scope, int new_cap) {
    const CBMVarBinding **table =
        (const CBMVarBinding **)cbm_arena_alloc(scope->arena, (size_t)new_cap * sizeof(*table));
    if (!table) {
        return false;
    }
    memset((void *)table, 0, (size_t)new_cap * sizeof(*table));
    for (const CBMScopeChunk *c = scope->chunks; c != NULL; c = c->next) {
        for (int i = 0; i < c->used; i++) {
            scope_index_insert(table, new_cap, &c->bindings[i],
                               cbm_scope_name_hash(c->bindings[i].name));
        }
    }
    scope->index = table;
    scope->index_cap = new_cap;
    return true;
}

/* Record a binding just appended to the chunks in the name index, creating or
 * growing the index as the scope fills. Load stays at or below one half. */
static void scope_index_add(CBMScope *scope, const CBMVarBinding *b, uint32_t hash) {
    if (scope->index_failed || scope->count <= CBM_SCOPE_INDEX_MIN_BINDINGS) {
        return;
    }
    if (!scope->index || scope->count * 2 > scope->index_cap) {
        int new_cap = scope->index_cap > 0 ? scope->index_cap * 2 : 128;
        if (new_cap <= scope->index_cap || !scope_index_rebuild(scope, new_cap)) {
            /* Without a complete index every reader must scan the chunks. */
            scope->index = NULL;
            scope->index_cap = 0;
            scope->index_failed = true;
        }
        return; /* a rebuild already covers b: it is in the chunks */
    }
    scope_index_insert(scope->index, scope->index_cap, b, hash);
}

/* Returns false when the binding could NOT be recorded in THIS frame.
 *
 * The failure that matters is arena exhaustion in alloc_chunk: the old void
 * form returned silently, so a caller that then consulted the scope CHAIN saw
 * the parent's binding for the same name and concluded the child had been
 * bound. For callable-value proof that is a fabricated identity -- the shadow
 * never took effect, yet the parent's callable looks like the child's. Callers
 * needing that distinction must use the checked form and consult the LOCAL
 * result, not a chain lookup. */
static bool cbm_scope_bind_value(CBMScope *scope, const char *name, const CBMType *type,
                                 const char *callable_qn) {
    if (!scope || !name) {
        return false;
    }
    uint32_t hash = cbm_scope_name_hash(name);
    CBMVarBinding *existing = cbm_scope_find_local(scope, name, hash);
    if (existing) {
        existing->type = type;
        existing->callable_qn = callable_qn;
        return true;
    }
    CBMScopeChunk* head = scope->chunks;
    if (!head || head->used >= CBM_SCOPE_CHUNK_BINDINGS) {
        head = alloc_chunk(scope);
        if (!head) {
            return false; /* arena exhausted: the shadow did NOT take effect */
        }
    }
    CBMVarBinding *added = &head->bindings[head->used];
    added->name = name;
    added->type = type;
    added->callable_qn = callable_qn;
    head->used++;
    scope->count++;
    scope_index_add(scope, added, hash);
    return true;
}

void cbm_scope_bind(CBMScope *scope, const char *name, const CBMType *type) {
    (void)cbm_scope_bind_value(scope, name, type, NULL);
}

bool cbm_scope_bind_checked(CBMScope *scope, const char *name, const CBMType *type) {
    return cbm_scope_bind_value(scope, name, type, NULL);
}

void cbm_scope_bind_callable(CBMScope *scope, const char *name, const CBMType *type,
                             const char *callable_qn) {
    (void)cbm_scope_bind_value(scope, name, type, callable_qn);
}

bool cbm_scope_bind_callable_checked(CBMScope *scope, const char *name, const CBMType *type,
                                     const char *callable_qn) {
    return cbm_scope_bind_value(scope, name, type, callable_qn);
}

const CBMType* cbm_scope_lookup(const CBMScope* scope, const char* name) {
    const CBMVarBinding *binding = cbm_scope_lookup_binding(scope, name);
    if (binding)
        return binding->type;
    return cbm_type_unknown();
}

bool cbm_scope_contains(const CBMScope *scope, const char *name) {
    return cbm_scope_lookup_binding(scope, name) != NULL;
}

const char *cbm_scope_lookup_callable(const CBMScope *scope, const char *name) {
    const CBMVarBinding *binding = cbm_scope_lookup_binding(scope, name);
    return binding ? binding->callable_qn : NULL;
}

bool cbm_scope_update_callable(CBMScope *scope, const char *name, const char *callable_qn) {
    if (!name) {
        return false;
    }
    CBMVarBinding *binding = (CBMVarBinding *)cbm_scope_lookup_binding(scope, name);
    if (!binding) {
        return false;
    }
    binding->callable_qn = callable_qn;
    return true;
}

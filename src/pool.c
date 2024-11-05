#include <stdlib.h>

#include "ctest_impl.h"

struct ctest_pool {
    struct test* pool;
    size_t size, cap;
};

struct suite_impl {
    const char* name;
    struct ctest_pool pool;
};

struct suite_pool {
    struct suite_impl* pool;
    size_t size, cap;
};

static struct suite_pool suites;

static void* alloc(void* p, size_t n) { return p ? realloc(p, n) : malloc(n); }

static void add_test(struct suite_impl* s, struct test t) {
    if (s->pool.size >= s->pool.cap) {
        s->pool.cap = s->pool.cap * 2 + 1;
        s->pool.pool = alloc(s->pool.pool, s->pool.cap * sizeof(struct test));
    }
    s->pool.pool[s->pool.size++] = t;
}

static struct suite_impl* add_suite(const char* name) {
    if (suites.size >= suites.cap) {
        suites.cap = suites.cap * 2 + 1;
        suites.pool = alloc(suites.pool, suites.cap * sizeof(struct test));
    }
    struct suite_impl* s = &suites.pool[suites.size++];
    s->pool = (struct ctest_pool){};
    s->name = name;
    return s;
}

static struct suite_impl* find_suite(const char* name) {
    struct suite_impl* s;
    for (s = suites.pool; s < suites.pool + suites.size; s++) {
        if (s->name == name) {
            return s;
        }
    }
    return add_suite(name);
}

void _ctest_add(struct test t) {
    struct suite_impl* s = find_suite(t.suite);
    add_test(s, t);
}

struct suite _ctest_suite_iter(suite_iter* it) {
    if (*it >= suites.size) {
        return (struct suite){};
    }
    struct suite_impl* s = &suites.pool[(*it)++];
    add_test(s, (struct test){});
    s->pool.size--;
    return (struct suite){.name = s->name, .iterator = s->pool.pool};
}

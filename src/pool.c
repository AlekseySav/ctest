#include <stdlib.h>

#include "ctest_impl.h"

struct ctest_pool {
    struct test* pool;
    size_t size, cap;
};

static struct ctest_pool tests;

static void* alloc(void* p, size_t n) { return p ? realloc(p, n) : malloc(n); }

void _ctest_add(struct test t) {
    if (tests.size >= tests.cap) {
        tests.cap = tests.cap * 2 + 1;
        tests.pool = alloc(tests.pool, tests.cap * sizeof(struct test));
    }
    tests.pool[tests.size++] = t;
}

struct test* _ctest_test_iter(void) {
    _ctest_add((struct test){});
    tests.size--;
    return tests.pool;
}

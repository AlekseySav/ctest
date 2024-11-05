#pragma once

#include <stddef.h>

#include "ctest.h"

struct test {
    const char* suite;
    const char* name;
    void (*fn)(void);
    int timeout;
    int exit_code;
    int waiter_pid;
};

struct suite {
    const char* name;
    struct test* iterator;
};

typedef int suite_iter;

/*
 * test pool
 */

void _ctest_add(struct test t);
struct suite _ctest_suite_iter(suite_iter* it);

/*
 * test executor
 */

void _ctest_exec(struct test* t);
void _ctest_stop(struct suite s);

/*
 * ctest
 */

void _ctest_reg(void (*fn)(void), const char* suite, const char* name, ...);
void _ctest_fail(const char* expr, const char* file, int line);
void _ctest_timeout(int timeout);

#pragma once

#include <stddef.h>

#include "test.h"

struct test {
    const char* suite;
    const char* name;
    void (*fn)(void);
    int timeout;
    int exit_code;
    int waiter_pid;
};

/*
 * test pool
 */

void _ctest_add(struct test t);
struct test* _ctest_test_iter(void);

/*
 * test executor
 */

void _ctest_exec(struct test* t);
void _ctest_stop(void);

/*
 * ctest
 */

void _ctest_reg(void (*fn)(void), const char* suite, const char* name, ...);
void _ctest_fail(const char* expr, const char* file, int line);
void _ctest_timeout(int timeout);

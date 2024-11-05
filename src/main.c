#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "ctest_impl.h"

void _ctest_reg(void (*fn)(void), const char* suite, const char* name, ...) {
    va_list ap;
    struct ctest_option opt;
    struct test t = (struct test){
        .suite = suite,
        .name = name,
        .fn = fn,
        .timeout = 10,
    };

    va_start(ap, name);
    for (;;) {
        opt = va_arg(ap, struct ctest_option);
        switch (opt.type) {
            case CTestTimeout:
                t.timeout = opt.value;
                break;
            case CTestEnd:
                _ctest_add(t);
                va_end(ap);
                return;
        }
    }
}

#define print(...) fprintf(stderr, __VA_ARGS__)

void _ctest_fail(const char* expr, const char* file, int line) {
    print("\e[31;1mAssertion failed: \e[0m%s:%d: (%s)\n", file, line, expr);
    abort();
}

void _ctest_timeout(int timeout) {
    print("\e[31;1mTest failed with timeout (%d seconds)\e[0m\n", timeout);
}

void ctest_run(void) {
    struct suite s;
    for (suite_iter it = 0; (s = _ctest_suite_iter(&it)).name;) {
        int total = 0, failed = 0;
        print("running test suite \e[36m%s\e[0m...\n", s.name);
        for (struct test* t = s.iterator; t->name; t++) {
            total++;
            _ctest_exec(t);
            if (t->exit_code != 0) {
                print("Test \e[36m%s:%s \e[31;1mfailed\e[0m with %#x\n",
                      t->suite, t->name, t->exit_code);
                failed++;
            }
        }
        if (failed) {
            print("\e[31;1mFAILED %d/%d\e[0m\n\n", failed, total);
        } else {
            print("\e[32;1mOK %d/%d\e[0m\n\n", total, total);
        }
        _ctest_stop(s);
    }

    int n_suites = 0, n_suites_failed = 0;
    for (suite_iter it = 0; (s = _ctest_suite_iter(&it)).name;) {
        n_suites++;
        for (struct test* t = s.iterator; t->name; t++) {
            if (t->exit_code) {
                n_suites_failed++;
                break;
            }
        }
    }
    if (n_suites_failed) {
        print("\e[31;1m%d/%d FAILED\e[0m\n", n_suites_failed, n_suites);
    } else {
        print("\e[32;1m%d/%d PASSED\e[0m\n", n_suites, n_suites);
    }
}

int main(int argc, char** argv) {
    ctest_run();
    return 0;
}

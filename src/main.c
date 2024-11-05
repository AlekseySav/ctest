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
    print("\e[031;1mAssertion failed: \e[0m%s:%d: (%s)\n", file, line, expr);
    abort();
}

void _ctest_timeout(int timeout) {
    print("\e[031;1mTest failed with timeout (%d seconds)\e[0m\n", timeout);
}

void ctest_run(void) {
    for (struct test* t = _ctest_test_iter(); t->name; t++) {
        print("Running test %s:%s\n", t->suite, t->name);
        _ctest_exec(t);
        if (t->exit_code != 0) {
            print("\e[031;1mTest %s:%s failed with code %#x\e[0m\n", t->suite,
                  t->name, t->exit_code);
        }
    }
    _ctest_stop();
}

int main(int argc, char** argv) {
    ctest_run();
    return 0;
}

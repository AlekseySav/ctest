#pragma once

/*
 * ctest interface
 */

#define TEST(suite, name, ...)                                             \
    _CTEST_REGISTER(test_##suite##_##name, suite,                          \
                    name __VA_OPT__(, ) __VA_ARGS__, _CTEST_OPT(CTestEnd)) \
    static void test_##suite##_##name(void)

#define TIMEOUT(seconds) _CTEST_OPT(CTestTimeout, (seconds))

#define ASSERT(e)                                        \
    do {                                                 \
        void _ctest_fail(const char*, const char*, int); \
        if (!(e)) {                                      \
            _ctest_fail(#e, __FILE__, __LINE__);         \
        }                                                \
    } while (0)

void ctest_run(void);

/*
 * ctest impl
 */

#define _CTEST_CNT(c) _CTEST_CNT_A(c, __COUNTER__)
#define _CTEST_CNT_A(a, b) _CTEST_CNT_B(a, b)
#define _CTEST_CNT_B(a, b) a##b

#define _CTEST_REGISTER(fn, suite, name, ...)                              \
    static void fn(void);                                                  \
    __attribute__((constructor)) static void _CTEST_CNT(_ctest_r_)(void) { \
        void _ctest_reg(void (*)(void), const char*, const char*, ...);    \
        _ctest_reg(fn, #suite, #name, __VA_ARGS__);                        \
    }

#define _CTEST_OPT(...) ((struct ctest_option){__VA_ARGS__})

struct ctest_option {
    enum {
        CTestEnd,
        CTestTimeout,
    } type;

    int value;
};

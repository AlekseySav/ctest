#include <unistd.h>

#define _CTEST_IMPL
#include "ctest.h"

int add(int a, int b) { return a + b; }

int faulty_add(int a, int b) { return a - b; }

int slow_add(int a, int b) {
    sleep(5);
    return a + b;
}

TEST(ok, add) {
    ASSERT(add(2, 2) == 4);       // ok
    ASSERT(slow_add(2, 2) == 4);  // ok
}

TEST(fail, assert) {
    ASSERT(faulty_add(2, 2) == 4);  // fail
}

TEST(fail, timeout, TIMEOUT(1)) {
    ASSERT(slow_add(2, 2) == 4);  // timeout
}

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ctest_impl.h"

void _ctest_exec(struct test* t) {
    int worker_pid = fork();
    if (worker_pid < 0) {
        perror(strerror(errno));
        exit(1);
    }
    if (worker_pid == 0) {
        t->fn();
        exit(0);
    }

    int waiter_pid = fork();
    if (waiter_pid < 0) {
        kill(worker_pid, SIGTERM);
        perror(strerror(errno));
        exit(1);
    }
    if (waiter_pid == 0) {
        sleep(t->timeout);
        kill(worker_pid, SIGTERM);
        _ctest_timeout(t->timeout);
        exit(0);
    }

    t->waiter_pid = waiter_pid;
    waitpid(worker_pid, &t->exit_code, 0);
}

void _ctest_stop(void) {
    for (struct test* t = _ctest_test_iter(); t->name; t++) {
        kill(t->waiter_pid, SIGTERM);
    }
}

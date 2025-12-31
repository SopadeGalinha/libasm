#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "test_common.h"

typedef size_t (*t_strlen)(const char *);

static int expect_segv(t_strlen f) {
    pid_t pid = fork();
    if (pid == 0) {
        f(NULL);
        _exit(0);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV;
}

int test_ft_strlen(void *handle) {
    int fails = 0;
    t_strlen f = (t_strlen)load_sym(handle, "ft_strlen");
    if (!f) return 1; // missing
    line_head("ft_strlen");
    fails += tag(f("") == 0);
    fails += tag(f("hello") == 5);
    fails += tag(f("ab\0c") == 2);
    fails += tag(f("A") == 1);
    fails += tag(f("with spaces") == 11);
    char big[256]; memset(big, 'x', sizeof(big)); big[255] = '\0';
    fails += tag(f(big) == 255);
    char big2[1024]; memset(big2, 'y', sizeof(big2)); big2[1023] = '\0';
    fails += tag(f(big2) == 1023);
    fails += tag(f("trailing\0ignored") == 8);
    fails += tag(f("repeat") == 6); // stability/repeatability
    fails += tag(expect_segv(f));
    line_tail();
    return fails;
}

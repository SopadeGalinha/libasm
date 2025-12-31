#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "test_common.h"

typedef ssize_t (*t_write)(int, const void *, size_t);

int test_ft_write(void *handle) {
    int fails = 0;
    t_write f = (t_write)load_sym(handle, "ft_write");
    if (!f) return 1;
    line_head("ft_write");
    int fds[2]; pipe(fds);
    const char *msg = "hi";
    ssize_t w = f(fds[1], msg, 2);
    char buf[4] = {0};
    read(fds[0], buf, sizeof(buf));
    fails += tag(w == 2);
    fails += tag(strcmp(buf, "hi") == 0);
    errno = 0; w = f(-1, msg, 2);
    fails += tag(w == -1 && errno == EBADF);
    errno = 0; w = f(fds[1], NULL, 1);
    fails += tag(w == -1 && errno == EFAULT);
    w = f(fds[1], msg, 0);
    fails += tag(w == 0);
    char big[512]; memset(big, 'Z', sizeof(big));
    w = f(fds[1], big, sizeof(big));
    fails += tag(w == (ssize_t)sizeof(big));
    close(fds[0]); close(fds[1]);
    int p[2]; pipe(p);
    close(p[0]); // closed read end
    errno = 0; w = f(p[1], "x", 1);
    fails += tag((w == -1 && errno == EPIPE) || w == 1); // platform dependent
    close(p[1]);
    errno = 0; w = f(1, "", 0);
    fails += tag(w == 0);
    errno = 0; w = f(1, "A", 1);
    fails += tag(w == 1);
    errno = 123; w = f(1, "B", 0);
    fails += tag(w == 0 && errno == 123); // success must not clobber errno
    close(fds[0]); close(fds[1]);
    line_tail();
    return fails;
}

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "test_common.h"

typedef ssize_t (*t_read)(int, void *, size_t);

int test_ft_read(void *handle) {
    int fails = 0;
    t_read f = (t_read)load_sym(handle, "ft_read");
    if (!f) return 1;
    line_head("  ft_read");
    int fds[2]; pipe(fds);
    const char *msg = "hello";
    write(fds[1], msg, 5);
    char buf[8] = {0};
    ssize_t r = f(fds[0], buf, 5);
    fails += tag(r == 5 && strncmp(buf, "hello", 5) == 0);
    errno = 0; r = f(-1, buf, 1);
    fails += tag(r == -1 && errno == EBADF);
    errno = 0; r = f(fds[0], NULL, 1);
    fails += tag(r == -1 && errno == EFAULT);
    r = f(fds[0], buf, 0);
    fails += tag(r == 0);
    int fds2[2]; pipe(fds2);
    write(fds2[1], "a", 1);
    close(fds2[1]);
    r = f(fds2[0], buf, 4);
    fails += tag(r == 1);
    r = f(fds2[0], buf, 4);
    fails += tag(r == 0); // EOF now
    close(fds2[0]);
    int fds3[2]; pipe(fds3);
    close(fds3[1]);
    r = f(fds3[0], buf, 1);
    fails += tag(r == 0); // empty pipe, EOF
    close(fds3[0]);
    int fds4[2]; pipe(fds4);
    write(fds4[1], "XY", 2);
    r = f(fds4[0], buf, 1);
    fails += tag(r == 1 && buf[0] == 'X');
    r = f(fds4[0], buf, 1);
    fails += tag(r == 1 && buf[0] == 'Y');
    close(fds4[0]); close(fds4[1]);
    int devnull = open("/dev/null", O_RDONLY);
    r = f(devnull, buf, 4);
    fails += tag(r == 0); // /dev/null returns EOF immediately
    close(devnull);
    close(fds[0]); close(fds[1]);
    line_tail();
    return fails;
}

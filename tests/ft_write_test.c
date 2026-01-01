#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "test_common.h"

typedef ssize_t (*t_write)(int, const void *, size_t);

int test_ft_write(void *handle) {
    int fails = 0;
    t_write f = (t_write)load_sym(handle, "ft_write");
    if (!f) return 1;

    line_head(" ft_write");

    /* 1-2: basic successful write/read via pipe */
    int p1[2];
    pipe(p1);
    const char *msg = "hi";
    ssize_t w = f(p1[1], msg, 2);
    char buf[260] = {0};
    read(p1[0], buf, sizeof(buf));
    fails += tag(w == 2);
    fails += tag(strcmp(buf, "hi") == 0);
    close(p1[0]);
    close(p1[1]);

    /* 3-5: large write round-trip via pipe */
    int p2[2];
    pipe(p2);
    char big[256]; memset(big, 'Z', sizeof(big));
    memset(buf, 0, sizeof(buf));
    w = f(p2[1], big, sizeof(big));
    ssize_t r = read(p2[0], buf, sizeof(big));
    fails += tag(w == (ssize_t)sizeof(big));
    fails += tag(r == (ssize_t)sizeof(big));
    fails += tag(memcmp(buf, big, sizeof(big)) == 0);
    close(p2[0]);
    close(p2[1]);

    /* 6: EBADF on invalid fd */
    errno = 0;
    w = f(-1, msg, 2);
    fails += tag(w == -1 && errno == EBADF);

    /* 7: zero-length write returns 0 without clobbering errno */
    errno = 123;
    w = f(STDOUT_FILENO, "", 0);
    fails += tag(w == 0 && errno == 123);

    /* 8: write to /dev/null succeeds */
    int dn = open("/dev/null", O_WRONLY);
    w = f(dn, "xyz", 3);
    fails += tag(w == 3);
    close(dn);

    /* 9-10: success does not change errno and data matches */
    int p3[2];
    pipe(p3);
    errno = 77;
    w = f(p3[1], "A", 1);
    char chk[2] = {0};
    read(p3[0], chk, 1);
    fails += tag(w == 1 && errno == 77);
    fails += tag(chk[0] == 'A');
    close(p3[0]);
    close(p3[1]);

    line_tail();
    return fails;
}

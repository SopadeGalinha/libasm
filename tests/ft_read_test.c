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

    char buf[16] = {0};

    /* 1-2: basic successful read from pipe */
    int p1[2];
    pipe(p1);
    const char *msg = "hello";
    write(p1[1], msg, 5);
    ssize_t r = f(p1[0], buf, 5);
    fails += tag(r == 5);
    fails += tag(strncmp(buf, "hello", 5) == 0);
    close(p1[0]);
    close(p1[1]);

    /* 3-4: partial then remaining read (combined checks) */
    int p2[2];
    pipe(p2);
    write(p2[1], "abcdef", 6);
    memset(buf, 0, sizeof(buf));
    r = f(p2[0], buf, 3);
    fails += tag(r == 3 && strncmp(buf, "abc", 3) == 0);
    memset(buf, 0, sizeof(buf));
    r = f(p2[0], buf, 4);
    fails += tag(r == 3 && strncmp(buf, "def", 3) == 0);
    close(p2[0]);
    close(p2[1]);

    /* 5: EBADF on invalid fd */
    errno = 0;
    r = f(-1, buf, 1);
    fails += tag(r == -1 && errno == EBADF);

    /* 6: zero-length read returns 0 without clobbering errno */
    errno = 99;
    r = f(STDIN_FILENO, buf, 0);
    fails += tag(r == 0 && errno == 99);

    /* 7: /dev/null returns 0 (EOF) */
    int dn = open("/dev/null", O_RDONLY);
    r = f(dn, buf, 4);
    fails += tag(r == 0);
    close(dn);

    /* 8: EOF on pipe with write end closed */
    int p3[2];
    pipe(p3);
    close(p3[1]);
    r = f(p3[0], buf, 4);
    fails += tag(r == 0);
    close(p3[0]);

    /* 9-10: success does not clobber errno and data matches */
    int p4[2];
    pipe(p4);
    write(p4[1], "X", 1);
    errno = 55;
    memset(buf, 0, sizeof(buf));
    r = f(p4[0], buf, 1);
    fails += tag(r == 1 && errno == 55);
    fails += tag(buf[0] == 'X');
    close(p4[0]);
    close(p4[1]);

    line_tail();
    return fails;
}

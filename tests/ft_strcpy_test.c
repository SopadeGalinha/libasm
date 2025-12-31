#include <string.h>
#include "test_common.h"

typedef char *(*t_strcpy)(char *, const char *);

int test_ft_strcpy(void *handle) {
    int fails = 0;
    t_strcpy f = (t_strcpy)load_sym(handle, "ft_strcpy");
    if (!f) return 1;
    line_head("ft_strcpy");
    char buf[32];
    fails += tag(strcmp(f(buf, ""), "") == 0);
    fails += tag(strcmp(f(buf, "hello"), "hello") == 0);
    char src[] = "abc\0def";
    f(buf, src);
    fails += tag(strcmp(buf, "abc") == 0);
    char buf2[4];
    fails += tag(strcmp(f(buf2, "ab"), "ab") == 0);
    fails += tag(strcmp(f(buf, "xyz"), "xyz") == 0);
    char *ret = f(buf, "single");
    fails += tag(ret == buf);
    char longsrc[200]; memset(longsrc, 'k', sizeof(longsrc)); longsrc[199] = '\0';
    fails += tag(strcmp(f(buf, longsrc), longsrc) == 0);
    char small[2];
    fails += tag(strcmp(f(small, "a"), "a") == 0);
    char prefill[8]; memset(prefill, 'Z', sizeof(prefill)); f(prefill, "");
    fails += tag(prefill[0] == '\0');
    line_tail();
    return fails;
}

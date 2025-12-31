#include <string.h>
#include "test_common.h"

typedef int (*t_strcmp)(const char *, const char *);

int test_ft_strcmp(void *handle) {
    int fails = 0;
    t_strcmp f = (t_strcmp)load_sym(handle, "ft_strcmp");
    if (!f) return 1;
    line_head("ft_strcmp");
    fails += tag(f("abc", "abc") == 0);
    fails += tag(f("abc", "abd") < 0);
    fails += tag(f("abe", "abd") > 0);
    fails += tag(f("", "a") < 0);
    fails += tag(f("a\0z", "a\0y") == 0);
    fails += tag(f("", "") == 0);
    fails += tag(f("abc", "abcd") < 0);
    fails += tag(f("abcd", "abc") > 0);
    fails += tag(f("Case", "case") < 0);
    fails += tag(f("\xFF", "\x7F") > 0); // unsigned compare
    line_tail();
    return fails;
}

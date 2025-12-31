#include <stdlib.h>
#include <string.h>
#include "test_common.h"

typedef char *(*t_strdup)(const char *);

int test_ft_strdup(void *handle) {
    int fails = 0;
    t_strdup f = (t_strdup)load_sym(handle, "ft_strdup");
    if (!f) return 1;
    line_head("ft_strdup");
    char *s1 = f("hello");
    fails += tag(s1 && strcmp(s1, "hello") == 0); free(s1);
    char *s2 = f("");
    fails += tag(s2 && strcmp(s2, "") == 0); free(s2);
    char *s3 = f("a\0b");
    fails += tag(s3 && strcmp(s3, "a") == 0); free(s3);
    char big[300]; memset(big, 'q', 299); big[299] = '\0';
    char *s4 = f(big);
    fails += tag(s4 && strlen(s4) == 299); free(s4);
    char *s5 = f("repeat");
    char *s6 = f("repeat");
    fails += tag(s5 && s6 && s5 != s6); free(s5); free(s6);
    char *s7 = f("x");
    fails += tag(s7 && s7[0] == 'x' && s7[1] == '\0'); free(s7);
    char src_mut[] = "mutable";
    char *copy = f(src_mut);
    src_mut[0] = 'X';
    fails += tag(copy && strcmp(copy, "mutable") == 0); free(copy);
    char long2[512]; memset(long2, 'p', sizeof(long2)); long2[511] = '\0';
    char *s8 = f(long2);
    fails += tag(s8 && strlen(s8) == 511); free(s8);
    char *s9 = f("end");
    fails += tag(s9 && s9[strlen(s9)] == '\0'); free(s9);
    line_tail();
    return fails;
}

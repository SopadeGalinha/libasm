#pragma once
#include <stdio.h>
#include <dlfcn.h>

#define CLR_RESET "\033[0m"
#define CLR_RED   "\033[0;31m"
#define CLR_GREEN "\033[0;32m"
#define CLR_YEL   "\033[0;33m"

static inline void *load_sym(void *handle, const char *name) {
    dlerror();
    void *p = dlsym(handle, name);
    if (!p) {
        const char *err = dlerror();
        printf(CLR_YEL "[MISS]" CLR_RESET " %s%s%s%s\n", name, err ? " (" : "", err ? err : "", err ? ")" : "");
    }
    return p;
}

static inline int check(int cond, const char *label) {
    printf("%s[%s]%s %s\n", cond ? CLR_GREEN : CLR_RED, cond ? "OK" : "KO", CLR_RESET, label);
    return cond ? 0 : 1;
}

static inline void line_head(const char *label) {
    printf("%s:%s ", label, label && *label ? "" : "");
}

static inline int tag(int cond) {
    printf("%s[%s]%s", cond ? CLR_GREEN : CLR_RED, cond ? "OK" : "KO", CLR_RESET);
    return cond ? 0 : 1;
}

static inline void line_tail(void) {
    putchar('\n');
}

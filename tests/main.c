#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int test_ft_strlen(void *handle);
int test_ft_strcmp(void *handle);
int test_ft_strcpy(void *handle);
int test_ft_strdup(void *handle);
int test_ft_write(void *handle);
int test_ft_read(void *handle);

int main(void) {
    void *h = dlopen("./libasm.so", RTLD_LAZY);
    if (!h) {
        fprintf(stderr, "Failed to load libasm.so: %s\n", dlerror());
        return EXIT_FAILURE;
    }
    int fails = 0;
    fails += test_ft_strlen(h);
    fails += test_ft_strcmp(h);
    fails += test_ft_strcpy(h);
    fails += test_ft_strdup(h);
    fails += test_ft_write(h);
    fails += test_ft_read(h);
    dlclose(h);
    if (fails == 0) {
        printf("All tests passed.\n");
        return EXIT_SUCCESS;
    }
    printf("Tests finished with %d failure(s).\n", fails);
    return EXIT_FAILURE;
}

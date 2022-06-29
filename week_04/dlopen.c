#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>

// 提供了一种在运行时手工加载库的手段

int main(int argc, char const *argv[])
{
    void *handle = NULL;
    double (*cos)(double) = NULL;
    handle = dlopen(LIBM_SO, RTLD_LAZY);
    if (handle == NULL)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    dlerror();
    cos = dlsym(handle, "cos");
    // *(void **)(&cos) = dlsym(handle, "cos");
    if (cos == NULL)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    printf("%lf\n", cos(2.0));
    dlclose(handle);
    exit(EXIT_SUCCESS);
}

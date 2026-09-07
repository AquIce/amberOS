#ifndef __AMBER_STD_PANIC__
#define __AMBER_STD_PANIC__

_Noreturn void kpanic(
    const char* file,
    int line,
    const char* expr
);

#define kassert(expr) \
    do { \
        if (!(expr)) { \
            kernel_panic(__FILE__, __LINE__, #expr); \
		} \
    } while(0)

#endif // __AMBER_STD_PANIC__

#ifndef __AMBER_KERNEL_INTERRUPTS_FRAME__
#define __AMBER_KERNEL_INTERRUPTS_FRAME__

#include <base/foundation/macros.h>

typedef struct {
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 esp;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;

    u32 vector;
    u32 error_code;

    u32 eip;
    u32 cs;
    u32 eflags;
} InterruptFrame;

#endif // __AMBER_KERNEL_INTERRUPTS_FRAME__

#ifndef __AMBER_KERNEL_INTERRUPTS_PIT__
#define __AMBER_KERNEL_INTERRUPTS_PIT__

#include <kernel/io/in_out_wait.h>

#include <base/foundation/macros.h>

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_BASE_FREQ 1193182

void pit_init(u32 frequency);

#endif // __AMBER_KERNEL_INTERRUPTS_PIT__

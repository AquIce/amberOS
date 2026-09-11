#ifndef __AMBER_KERNEL_INTERRUPTS_IRQ_PIT__
#define __AMBER_KERNEL_INTERRUPTS_IRQ_PIT__

#include <kernel/interrupts/interrupt_frame.h>

#include <base/foundation/macros.h>

u32 pit_ticks(void);

void pit_irq(InterruptFrame* frame);

#endif // __AMBER_KERNEL_INTERRUPTS_IRQ_PIT__

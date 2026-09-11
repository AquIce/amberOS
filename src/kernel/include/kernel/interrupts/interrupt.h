#ifndef __AMBER_KERNEL_INTERRUPTS_INTERRUPT__
#define __AMBER_KERNEL_INTERRUPTS_INTERRUPT__

#include <kernel/interrupts/interrupt_frame.h>

__attribute__((noreturn))
void exception_handler(InterruptFrame* frame);

void irq_handler(InterruptFrame* frame);

void interrupt_dispatch(InterruptFrame* frame);

#endif // __AMBER_KERNEL_INTERRUPTS_INTERRUPT__

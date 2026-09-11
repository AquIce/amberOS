#ifndef __AMBER_KERNEL_INTERRUPTS_IRQ__
#define __AMBER_KERNEL_INTERRUPTS_IRQ__

#include <kernel/interrupts/interrupt_frame.h>

#include <base/foundation/macros.h>

#define IRQ_COUNT 16

typedef void (*IRQ_Handler)(InterruptFrame* frame);

void irq_init(void);

bool irq_register(u8 irq, IRQ_Handler handler);
bool irq_unregister(u8 irq);

void irq_handler(InterruptFrame *frame);

u32 pit_ticks(void);

#endif // __AMBER_KERNEL_INTERRUPTS_IRQ__

#include <kernel/interrupts/irq/pit_irq.h>

#include <kernel/interrupts/interrupt_frame.h>

#include <base/foundation/macros.h>

internal volatile u32 timer_ticks;

u32 pit_ticks(void) {
	return timer_ticks;
}

void pit_irq(InterruptFrame* frame) {
    (void)frame;
    timer_ticks++;
}


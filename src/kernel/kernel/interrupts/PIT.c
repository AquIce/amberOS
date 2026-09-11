#include <kernel/interrupts/interrupt_frame.h>
#include <kernel/interrupts/PIT.h>

#include <base/foundation/macros.h>

void pit_init(u32 frequency) {
    u16 divisor = PIT_BASE_FREQ / frequency;

    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, divisor >> 8);
}

#include <kernel/interrupts/IRQ.h>
#include <kernel/interrupts/pic_8259.h>
#include <std/terminal.h>

persistent IRQ_Handler irq_handlers[IRQ_COUNT];

internal void pit_irq(InterruptFrame* frame);
internal void keyboard_irq(InterruptFrame* frame);

void irq_init(void) {
	for(u8 i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i] = nullptr;
	}
	irq_register(0, &pit_irq);
	irq_register(1, &keyboard_irq);
}

bool irq_register(u8 irq, IRQ_Handler handler) {
	if(irq >= IRQ_COUNT || handler == nullptr) {
		return false;
	}

	irq_handlers[irq] = handler;
	return true;
}

bool irq_unregister(u8 irq) {
	if(irq >= IRQ_COUNT || irq_handlers[irq] == nullptr) {
		return false;
	}

	irq_handlers[irq] = nullptr;
	return true;
}

void irq_handler(InterruptFrame *frame) {
	u8 irq = frame->vector - 0x20;

	if(irq_handlers[irq] != nullptr) {
        irq_handlers[irq](frame);
	}

    PIC_sendEOI(irq);
}

internal volatile u32 timer_ticks;

u32 pit_ticks(void) {
	return timer_ticks;
}

internal void pit_irq(InterruptFrame* frame) {
    (void)frame;
    timer_ticks++;
}

internal void keyboard_irq(InterruptFrame* frame) {
    (void)frame;

    u8 scancode = inb(0x60);

    kterm_write("Key: ");
    kterm_write_hex(scancode);
    kterm_write("\n");
}

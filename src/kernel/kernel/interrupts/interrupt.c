#include <kernel/interrupts/interrupt.h>
#include <kernel/interrupts/IRQ.h>
#include <kernel/interrupts/pic_8259.h>
#include <std/terminal.h>

void exception_handler(InterruptFrame* frame) {
	kterm_write("Exception: ");
    kterm_write_dec(frame->vector);
    kterm_write("\n");

    kterm_write("Error code: ");
    kterm_write_hex(frame->error_code);
    kterm_write("\n");

    kterm_write("EIP: ");
    kterm_write_hex(frame->eip);
    kterm_write("\n");

	for(;;) {
		__asm__ volatile ( "cli; hlt" );
	}
}

void interrupt_dispatch(InterruptFrame *frame) {
	if(frame->vector < 32) {
        exception_handler(frame);
    }
	if(frame->vector < 48) {
        irq_handler(frame);
    }
}

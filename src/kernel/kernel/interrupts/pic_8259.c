#include <kernel/interrupts/pic_8259.h>

void PIC_sendEOI(u8 irq) {
	if(irq >= PIC_SIZE) {
		outb(PIC_COMMAND(PIC2),PIC_EOI);
	}
	
	outb(PIC_COMMAND(PIC1),PIC_EOI);
}

void PIC_remap(int offset1, int offset2) {
	// Starts initialization sequence (in cascade mode) on both PICs
	outb(PIC_COMMAND(PIC1), ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC_COMMAND(PIC2), ICW1_INIT | ICW1_ICW4);
	io_wait();

	// ICW2: Master PIC vector offset
	outb(PIC_DATA(PIC1), offset1);
	io_wait();
	// ICW2: Slave PIC vector offset
	outb(PIC_DATA(PIC2), offset2);
	io_wait();
	// ICW3: tell Master PIC that there is a slave PIC at IRQ2
	outb(PIC_DATA(PIC1), 1 << CASCADE_IRQ);
	io_wait();
	// ICW3: tell Slave PIC its cascade identity
	outb(PIC_DATA(PIC2), CASCADE_IRQ);
	io_wait();
	
	// ICW4: force PICs to use 8086 mode (and not 8080 mode)
	outb(PIC_DATA(PIC1), ICW4_8086);
	io_wait();
	outb(PIC_DATA(PIC2), ICW4_8086);
	io_wait();

	// Unmask both PICs.
	outb(PIC_DATA(PIC1), 0);
	outb(PIC_DATA(PIC2), 0);
}

void IRQ_set_mask(u8 IRQ_line) {
    u16 port;
    u8 value;

    if(IRQ_line < PIC_SIZE) {
        port = PIC_DATA(PIC1);
    } else {
        port = PIC_DATA(PIC2);
        IRQ_line -= 8;
    }
    value = inb(port) | (1 << IRQ_line);
    outb(port, value);
}

void IRQ_clear_mask(u8 IRQ_line) {
    u16 port;
    u8 value;

    if(IRQ_line < PIC_SIZE) {
        port = PIC_DATA(PIC1);
    } else {
        port = PIC_DATA(PIC2);
        IRQ_line -= 8;
    }
    value = inb(port) & ~(1 << IRQ_line);
    outb(port, value);
}

internal u16 __pic_get_irq_reg(int ocw3) {
    // OCW3 to PIC CMD to get the register values
	
    outb(PIC_COMMAND(PIC1), ocw3);
    outb(PIC_COMMAND(PIC2), ocw3);
    return (
		inb(PIC_COMMAND(PIC2)) << 8)
		| inb(PIC_COMMAND(PIC1)
	);
}

u16 pic_get_irr(void) {
    return __pic_get_irq_reg(PIC_READ_IRR);
}

u16 pic_get_isr(void) {
    return __pic_get_irq_reg(PIC_READ_ISR);
}

#ifndef __AMBER_KERNEL_INTERRUPTS_PIC_8259__
#define __AMBER_KERNEL_INTERRUPTS_PIC_8259__

#include <kernel/io/in_out_wait.h>

#define PIC_SIZE 8
#define PIC1 0x20 // Master PIC
#define PIC2 0xA0 // Slave PIC
#define PIC_COMMAND(PIC) (PIC)
#define PIC_DATA(PIC) (PIC + 1)

#define PIC_EOI 0x20

void PIC_sendEOI(u8 irq);

#define ICW1_ICW4		0x01 // Indicates that ICW4 will be present
#define ICW1_SINGLE		0x02 // Single (cascade) mode
#define ICW1_INTERVAL4	0x04 // Call address interval 4 (8)
#define ICW1_LEVEL		0x08 // Level triggered (edge) mode
#define ICW1_INIT		0x10 // Initialization - required!

#define ICW4_8086		0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO		0x02 // Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08 // Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C // Buffered mode/master
#define ICW4_SFNM		0x10 // Special fully nested (not)

#define CASCADE_IRQ 2

/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2);


void IRQ_set_mask(u8 IRQ_line);
void IRQ_clear_mask(u8 IRQ_line);

#define PIC_READ_IRR 0x0a // OCW3 irq ready next CMD read
#define PIC_READ_ISR 0x0b // OCW3 irq service next CMD read

// Returns the combined value of the cascaded PICs irq request register
u16 pic_get_irr(void);

// Returns the combined value of the cascaded PICs in-service register
u16 pic_get_isr(void);

#endif // __AMBER_KERNEL_INTERRUPTS_PIC_8259__

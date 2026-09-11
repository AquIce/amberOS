#include <kernel/interrupts/IDT.h>

#define IDT_MAX_DESCRIPTORS 256
#define IDTE_PERFORMANCE_ALIGNMENT 0x10
#define IDT_VECTOR_STUB_SIZE 32
#define IDT_CODE_SECTOR_SIZE 0x08

__attribute__((aligned(IDTE_PERFORMANCE_ALIGNMENT)))
persistent IDT_Entry32 idt[IDT_MAX_DESCRIPTORS];
persistent IDTR idtr;

void idt_set_descriptor(u8 vector, void* isr, u8 flags) {
    IDT_Entry32* descriptor = &idt[vector];

    descriptor->offset_low	= (u32)isr & 0xFFFF;
    descriptor->selector	= IDT_CODE_SECTOR_SIZE; // Code selector offset in GDT
    descriptor->attributes	= flags;
    descriptor->offset_high = (u32)isr >> 16;
    descriptor->zero		= 0;
}

persistent bool vectors[IDT_MAX_DESCRIPTORS];
extern void* interrupt_stub_table[];

void idt_init() {
    idtr.base = (uptr)&idt[0];
    idtr.limit = (u16)sizeof(IDT_Entry32) * IDT_MAX_DESCRIPTORS - 1;

    for (u8 vector = 0; vector < 48; vector++) {
        idt_set_descriptor(
			vector,
			interrupt_stub_table[vector],
			0x8E
		);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

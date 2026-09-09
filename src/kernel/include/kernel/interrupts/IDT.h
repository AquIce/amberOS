#ifndef __AMBER_KERNEL_INTERRUPTS_IDT__
#define __AMBER_KERNEL_INTERRUPTS_IDT__

#include <base/foundation/macros.h>

typedef struct {
	u16 offset_low;
	u16 selector;
	u8 zero;
	u8 attributes;
	u16 offset_high;
} __attribute__((packed)) IDT_Entry32;

typedef struct {
	u16 limit;
	u32 base;
} __attribute__((packed)) IDTR;

void idt_set_descriptor(u8 vector, void* isr, u8 flags);

void idt_init(void);

internal_fn u8 idte_make_attributes(u8 dpl, u8 gate_type) {
	return (
		1 << 7
		| (dpl & 0b11) << 5
		| (gate_type & 0b1111)
	);
}

#endif // __AMBER_KERNEL_INTERRUPTS_IDT__

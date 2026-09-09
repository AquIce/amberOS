#ifndef __AMBER_KERNEL_IO_IN_OUT_WAIT__
#define __AMBER_KERNEL_IO_IN_OUT_WAIT__

#include <base/foundation/macros.h>

internal_fn void outb(u16 port, u8 val) {
    __asm__ volatile (
		"outb %b0, %w1"
		:
		: "a"(val), "Nd"(port)
		: "memory"
	);
}

internal_fn u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile (
		"inb %w1, %b0"
		: "=a"(ret)
		: "Nd"(port)
		: "memory"
	);
    return ret;
}

internal_fn void io_wait(void) {
    outb(0x80, 0);
}

#endif // __AMBER_KERNEL_IO_IN_OUT_WAIT__

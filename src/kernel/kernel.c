#include <std/ktypes.h>
#include <std/terminal.h>
#include <std/kmem.h>
#include <kernel/interrupts/IDT.h>
#include <kernel/interrupts/pic_8259.h>

#include <base/foundation/memory/memory.h>

persistent MemorySource kernel_memory_source;

void kmain(void) {

	memory_init(&kernel_memory_source);

	idt_init();
	PIC_remap(0x20, 0x28);

	// Install IRQ handlers in IDT (vectors 32–47)
	// Configure devices/timer
	// `sti`

	kterm_init(&kernel_memory_source);

	kterm_write("Amber kernel\n");
	kterm_write("Initializing...\n");

	for(usize i = 1; i < TERMINAL_HEIGHT + 3; i++) {
		kterm_write_dec(i);
		kterm_endline();
	}

	for(;;) {
        __asm__ volatile ("cli; hlt");
    }
}

#include <std/ktypes.h>
#include <std/terminal.h>
#include <std/kmem.h>

#include <base/foundation/memory/memory.h>

persistent MemorySource kernel_memory_source;

void kmain(void) {

	memory_init(&kernel_memory_source);

	kterm_init(&kernel_memory_source);

	kterm_write("Amber kernel\n");
	kterm_write("Initializing...\n");

	for(usize i = 0; i < TERMINAL_HEIGHT + 5; i++) {
		kterm_write_dec(i);
		kterm_endline();
	}

	for(;;) {
        __asm__ volatile ("cli; hlt");
    }
}

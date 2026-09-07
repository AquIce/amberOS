#include <std/ktypes.h>
#include <std/terminal.h>
#include <std/kmem.h>

#include <base/foundation/memory/memory.h>

persistent MemorySource kernel_memory_source;

void kmain(void) {

	kterm_init();

	kterm_write("Amber kernel\n");
	kterm_write("Initializing...\n");

	memory_init(&kernel_memory_source);

	kterm_write("Address of first available u32: ");
	u32* ptr = MEMORY_SOURCE_RESERVE_T(u32, &kernel_memory_source);
	kterm_write_hex64((uptr)ptr);
	memory_source_release(&kernel_memory_source, ptr, sizeof(*ptr));
	kterm_endline();

	ptr = MEMORY_SOURCE_RESERVE_T(u32, &kernel_memory_source);
	kterm_write_hex64((uptr)ptr);
	kterm_endline();
	kterm_write_dec(*ptr);
	kterm_endline();
	*ptr = 1234;
	kterm_write_dec(*ptr);
	kterm_endline();
	kterm_write_dec(*((u32*)0x100014));
	kterm_endline();

	for(;;) {
        __asm__ volatile ("cli; hlt");
    }
}

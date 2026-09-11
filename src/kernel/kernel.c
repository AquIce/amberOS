#include <std/ktypes.h>
#include <std/terminal.h>
#include <std/kmem.h>
#include <kernel/interrupts/IDT.h>
#include <kernel/interrupts/IRQ.h>
#include <kernel/interrupts/pic_8259.h>
#include <kernel/interrupts/PIT.h>

#include <base/foundation/memory/memory.h>

persistent MemorySource kernel_memory_source;

void kmain(void) {

	memory_init(&kernel_memory_source);
	kterm_init(&kernel_memory_source);

	idt_init();

	pic_remap(0x20, 0x28);
	pit_init(100);
	irq_init();

	IRQ_clear_mask(0);

	kterm_write("Amber kernel\n");
	kterm_write("Initializing...\n");

	__asm__ volatile ("sti");

	for(;;) {
        __asm__ volatile ("hlt");
    }
}

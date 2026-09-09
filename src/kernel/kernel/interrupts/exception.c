#include <kernel/interrupts/exception.h>

void exception_handler() {
    __asm__ volatile ("cli; hlt");
	for(;;) {}
}

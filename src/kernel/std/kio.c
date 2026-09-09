#include <std/kio.h>

#include <std/terminal.h>

void panic(const char* message) {
	kterm_clear();
	kterm_write("--= KERNEL PANIC =--\n");
	kterm_write("Reason: ");
    kterm_write(message);
	kterm_endline();

    for (;;) {}
        asm volatile ("cli; hlt");
}

#include <std/panic.h>

#include <std/terminal.h>

_Noreturn void kernel_panic(
    const char* file,
    int line,
    const char* expr
) {
    kterm_write("KERNEL PANIC\n");
    kterm_write("assertion failed: ");
    kterm_write(expr);
	kterm_endline();

    kterm_write("at ");
    kterm_write(file);
    kterm_write_dec(line);
	kterm_endline();

    for (;;)
        asm volatile ("cli; hlt");
}

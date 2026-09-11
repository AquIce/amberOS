#include <kernel/interrupts/irq/keyboard_irq.h>

#include <kernel/io/in_out_wait.h>
#include <std/terminal.h>

#include <base/foundation/macros.h>

void keyboard_irq(InterruptFrame* frame) {
    (void)frame;

    u8 scancode = inb(0x60);

    kterm_write("Key: ");
    kterm_write_hex(scancode);
    kterm_write("\n");
}

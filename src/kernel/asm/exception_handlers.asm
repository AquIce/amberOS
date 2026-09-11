
%macro isr_no_err_stub 1
global isr_stub_%+%1
isr_stub_%+%1:
    push dword 0          ; fake error code
    push dword %1         ; interrupt number
    jmp isr_common
%endmacro

%macro isr_err_stub 1
global isr_stub_%+%1
isr_stub_%+%1:
	; error code is pushed by the CPU
    push dword %1         ; interrupt number
    jmp isr_common
%endmacro

%macro irq_stub 2
global irq_stub_%+%1
irq_stub_%+%1:
    push dword 0          ; fake error code
    push dword %2         ; IDT vector
    jmp isr_common
%endmacro

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

irq_stub 0,  0x20
irq_stub 1,  0x21
irq_stub 2,  0x22
irq_stub 3,  0x23
irq_stub 4,  0x24
irq_stub 5,  0x25
irq_stub 6,  0x26
irq_stub 7,  0x27
irq_stub 8,  0x28
irq_stub 9,  0x29
irq_stub 10, 0x2A
irq_stub 11, 0x2B
irq_stub 12, 0x2C
irq_stub 13, 0x2D
irq_stub 14, 0x2E
irq_stub 15, 0x2F

extern interrupt_dispatch

isr_common:
    pusha

    push esp
    call interrupt_dispatch
    add esp, 4

    popa

    add esp, 8
    iretd


global interrupt_stub_table
interrupt_stub_table:

%assign i 0
%rep 32
    dd isr_stub_%+i
%assign i i+1
%endrep

%assign i 0
%rep 16
    dd irq_stub_%+i
%assign i i+1
%endrep

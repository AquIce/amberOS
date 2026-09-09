bits 32

DATA_SEG equ 0x10
STACK_TOP equ 0x90000

global _start
extern kmain

_start:
    ; Set up data segments
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up kernel stack
    mov esp, STACK_TOP

    call kmain

.hang:
    cli
    hlt
    jmp .hang

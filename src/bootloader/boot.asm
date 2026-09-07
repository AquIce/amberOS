; src/bootloader/boot.asm

bits 16

org 0x7c00

CODE_SEG			equ 0x08
ACPI_FIELD_SIZE		equ 24
KERNEL_LOAD_ADDR	equ 0x1000
E820_COUNT_ADDR		equ 0x3FFE
E820_BUFFER			equ 0x4000
REAL_MODE_STACK_TOP equ 0x7000
KERNEL_SECTORS		equ 32
KERNEL_SEGMENT		equ KERNEL_LOAD_ADDR >> 4

start:
    cli

	; BIOS gives us the boot drive in DL.
    mov [boot_drive], dl

	call load_kernel

	; Holy shit, I had a bug because this code used to be in `detect_memory`,
	; which means it discarded the return address when changing the stack
	; base address.
	xor ax, ax
	mov ss, ax
	mov sp, REAL_MODE_STACK_TOP

	call detect_memory

    lgdt [gdt_descriptor]

    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Load protected-mode CS
    jmp CODE_SEG:KERNEL_LOAD_ADDR

; -------------------------
; Load kernel from disk
; -------------------------

load_kernel:

    mov ax, KERNEL_SEGMENT
    mov es, ax

    xor bx, bx              ; ES:BX = 0x1000:0

    mov ah, 0x02            ; BIOS: read sectors
    mov al, KERNEL_SECTORS  ; Number of sectors
    mov ch, 0               ; Cylinder 0
    mov cl, 2               ; Sector 2 (because 1-indexed)
    mov dh, 0               ; Head 0
    mov dl, [boot_drive]    ; Drive

    int 0x13

    jc error           ; Carry flag = error

	ret

; -------------------------
; Detect memory
; -------------------------

detect_memory:
	xor ax, ax
	mov es, ax
	mov di, E820_BUFFER

	xor bp, bp

	xor ebx, ebx
	mov edx, 0x534D4150
	mov eax, 0xE820
	mov [es:di + 20], dword 1 ; force a valid ACPI 3.X entry
	mov ecx, ACPI_FIELD_SIZE

	int 0x15
	jc short error

	mov edx, 0x0534D4150 ; Some BIOSes apparently trash this register
    cmp eax, edx
    jne short error

	test ebx, ebx
	je short error

	jmp .loop_entry

	.loop_iter:
		mov eax, 0xE820
		mov [es:di + 20], dword 1
		mov ecx, 24
		int 0x15
		jc short .loop_end ; carry set means "end of list already reached"
		mov edx, 0x534D4150

	.loop_entry:
		jcxz .skip_entry ; skip any 0 length entries
		cmp cl, 20 ; low 8 bytes of ecx (current entry size)
		jbe short .non_acpi_entry
		test byte [es:di + 20], 1 ; ignore bit set?
		je short .skip_entry

	.non_acpi_entry:
		mov ecx, [es:di + 8]
		or ecx, [es:di + 12] ; combine upper and lower 32 bits "hack"
		jz .skip_entry
		inc bp
		add di, ACPI_FIELD_SIZE

	.skip_entry:
		test ebx, ebx
		jne .loop_iter

	.loop_end:
		mov [E820_COUNT_ADDR], bp
		ret


error:
    cli
    hlt

; -----------------------
; GDT
; -----------------------

gdt_start:

gdt_null:
    dq 0

gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

; -----------------------
; GDTR
; -----------------------

gdt_descriptor:					; "fat pointer"
    dw gdt_end - gdt_start - 1	; gdt size
    dd gdt_start				; gdt_start

boot_drive db 0

times 510 - ($ - $$) db 0
dw 0xAA55

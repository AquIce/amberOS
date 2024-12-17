; src/bootloader/boot.asm

; ===== MAIN =====

[ORG 0x7c00]		; set origin address
	jmp main
	%include "print.inc"
	%include "debug.inc"
	%include "readsect.inc"

main:
	; General Setup

	xor ax, ax    		; ax=0 
   	mov ds, ax   		; ds=0 / origin for string
   	mov ss, ax   		; stack starts at 0
   	mov sp, 0x9c00   	; stack offset = 9c00 

	; Text Output

   	cld			; setup string operation (df=0, si/di incremented later)

	mov ax, 0xb800   	; address of text video memory
   	mov es, ax

   	mov si, msg   		; move message address to si (=> then ds:si is right)
   	call printstr		; show text string

	; Kernel Load

	xor ax, ax
	mov es, ax    ; ES <- 0
	mov cx, 1     ; cylinder 0, sector 1
	mov dx, 0x0080 ; DH (head) = 0, DL (drive) = 80h (0th hard disk)
	mov bx, 0x1000 ; segment offset of the buffer
	mov al, 0x01 ; AL = 01 (number of sectors to read)
	
	call readsect

	hang:
   		jmp hang

; ===== Data =====

msg   		db "Welcome to amberOS!", 0

times 510-($-$$) db 0
db 0x55
db 0xAA

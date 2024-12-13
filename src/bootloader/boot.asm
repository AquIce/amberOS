; src/bootloader/boot.asm

; ===== MAIN =====

[ORG 0x7c00]		; set origin address
	jmp main
	%include "print.inc"

main:
	xor ax, ax    		; ax=0 
   	mov ds, ax   		; ds=0 / origin for string
   	mov ss, ax   		; stack starts at 0
   	mov sp, 0x9c00   	; stack offset = 9c00 

   	cld			; setup string operation (df=0, si/di incremented later)

	mov ax, 0xb800   	; address of text video memory
   	mov es, ax

   	mov si, msg   		; move message address to si (=> then ds:si is right)
   	call printstr		; show text string

   	mov ax, 0xb800   	; ax=b8 (look at video memory)
   	
	mov gs, ax
   	mov bx, 0x0000  	; set offset to 0 ('H'=48 attrib=0F)
   		
	hexdmp:
		mov ax, [gs:bx]

   		mov word [reg16], ax 	; look at register (gs:bx)
   		push bx
		call printreg16
		pop bx

		inc bx
		cmp bx, 0x0b		; size of msg - 1
		jne hexdmp

	hang:
   		jmp hang

; ===== Data =====

msg   		db "Hello, World!", 0

times 510-($-$$) db 0
db 0x55
db 0xAA

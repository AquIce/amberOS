; src/bootloader/debug.inc

; ===== Functions ======

printreg16:
        mov di, outstr16        ; di=outstr16
        mov ax, [reg16]         ; ax=reg16
        mov si, hexstr          ; so=hexstr
        mov cx, 4               ; print 4 chars

        hexloop:
                rol ax, 4               ; move every byte left (leftmost -> rightmost)
                mov bx, ax              ; bx=ax
                and bx, 0x0f            ; keep only rightmost byte (previous leftmost)
                mov bl, [si + bx]       ; bl=hex char
                mov [di], bl		; move hex char to di (current char in outstr16)
		inc di			; di++
		dec cx			; cx--
		jnz hexloop		; loop while cx != 0

	mov si, outstr16	; Print outstr16
	call printstr

	ret

; ===== Data =====

hexstr		db 	'0123456789ABCDEF'
outstr16	db	'0000', 0
reg16		dw	0

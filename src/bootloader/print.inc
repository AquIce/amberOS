dochar:
	call printchar
printstr:
	lodsb         		; ds:si (string char) to AL
        cmp al, 0
        jne dochar              ; print char if not EOS
	add byte [ypos], 1      ; down one row | -> '\n'
        mov byte [xpos], 0      ; back to left |
        ret

printchar:
        mov ah, 0x0f            ; attrib = white (F) on black (0)
                                ; ax is ah _ al -> attrib _ char
        mov cx, ax              ; save char/attrib
        movzx ax, byte [ypos]   ; ax=y position
        mov dx, 160             ; 80 (line length) * 2 bytes (char/attrib)
        mul dx                  ; ax *= dx (each line is 160 bytes because of 2 bytes/char and 80 columns)
        movzx bx, byte [xpos]   ; bx=x position
        shl bx, 1               ; shift left (times 2) to take attribs into account

        mov di, 0               ; di=start of video memory
        add di, ax              ; add y offset
        add di, bx              ; add x offset

        mov ax, cx              ; restore char/attribute
        stosw                   ; mov ax to es:di (move char/attrib to screen location)
        add byte [xpos], 1      ; advance to right

        ret

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

xpos		db	0
ypos		db	0
hexstr		db 	'0123456789ABCDEF'
outstr16	db	'0000', 0
reg16		dw	0

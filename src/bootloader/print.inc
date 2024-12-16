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

; ===== Data =====

xpos		db	0
ypos		db	0

; src/bootloader/readsect.inc

; ===== Functions =====

tryreadsect:
	; TODO: Add multi-try	
	ret

; Read sector(s) into memory
; Parameters
;	al	: Number of sectors to load
;	ch	: Low eight bits of cylinder number
;	cl	: Sector number (bits 0-5), high two bits of cylinder number (bits 6 - 7)
;	dh	: head number
;	dl	: drive number
;	[es:bx]	: data buffer
readsect:
	mov ah, 0x02
	
	int 0x13

	ret	

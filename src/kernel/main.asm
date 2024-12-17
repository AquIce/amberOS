; src/kernel/main.asm

kmain:
	jmp kmain

times 512-($-$$) db 0


set architecture i8086

target remote :1234

break *0x7c00
break *0x7c31
break *0x7c33
break *0x1000
break kmain

continue

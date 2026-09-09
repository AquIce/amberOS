
set architecture i8086

target remote :1234

break *0x7c00
break kmain

continue

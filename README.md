# amberOS
amberOS is my try at the creation of a simple OS

```sh
export PATH="$HOME/opt/cross/bin:$PATH"
```

In GDB:
```sh
target remote :1234
break *0x7c00
c
set disassembly-flavor intel
set architecture i386
x/50i 0x7c00
```



# scripts/bootloader.sh

mkdir -p bin/bootloader/
cd src/bootloader/
nasm boot.asm -f bin -o ../../bin/bootloader/boot.bin
cd ../..
qemu-system-i386 -hda bin/bootloader/boot.bin

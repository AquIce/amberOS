# scripts/bootloader.sh

# ===== GENERAL =====

mkdir -p bin/disks

# ===== BOOTLOADER =====

mkdir -p bin/bootloader/
cd src/bootloader/

nasm boot.asm -f bin -o ../../bin/bootloader/boot.bin
cd ../../
dd if=bin/bootloader/boot.bin of=bin/disks/disk.flp

# ===== KERNEL ===== 

mkdir -p bin/kernel
cd src/kernel

nasm main.asm -f bin -o ../../bin/kernel/main.bin
cd ../../
dd if=bin/kernel/main.bin of=bin/disks/disk.flp conv=notrunc oflag=append

# ===== END =====

qemu-system-i386 -drive format=raw,unit=0,file=bin/disks/disk.flp -name amberOS

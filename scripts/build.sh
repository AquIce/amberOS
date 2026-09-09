#!/bin/bash

set -e

# ===== CONFIG =====

CROSS="$HOME/opt/cross/bin/i686-elf-"
CC="${CROSS}gcc"
LD="${CROSS}ld"
OBJCOPY="${CROSS}objcopy"

# ===== OPTIONS =====

DEBUG=false

case "$1" in
    --debug)
        DEBUG=true
        ;;
    "")
        ;;
    *)
        echo "Usage: $0 [--debug]"
        exit 1
        ;;
esac

# ===== DIRECTORIES =====

mkdir -p bin/bootloader
mkdir -p bin/kernel
mkdir -p bin/kernel/asm
mkdir -p bin/disks

# ===== BOOTLOADER =====

echo "[1/6] Building bootloader..."

nasm \
    -f bin \
    src/bootloader/boot.asm \
    -o bin/bootloader/boot.bin

# ===== KERNEL ENTRY =====

echo "[2/6] Building kernel entry..."

nasm \
    -f elf32 \
    -g -F dwarf \
    src/kernel/asm/entry.asm \
    -o bin/kernel/asm/entry.o

# ===== KERNEL ASM =====

echo "[3/6] Building kernel ASM..."

nasm \
    -f elf32 \
    -g -F dwarf \
    src/kernel/asm/exception_handlers.asm \
    -o bin/kernel/asm/exception_handlers.o

# ===== KERNEL C =====

echo "[4/6] Building kernel C..."

KERNEL_OBJECTS=()

while IFS= read -r -d '' source; do
    relative="${source#src/kernel/}"
    object="bin/kernel/${relative%.c}.o"

    mkdir -p "$(dirname "$object")"

    echo "  CC $source"

    "$CC" \
        -m32 \
        -ffreestanding \
        -fno-pie \
        -fno-stack-protector \
        -Wall \
        -Wextra \
        -g \
        -I src/kernel/include \
        -I src/kernel/base/include \
        -c "$source" \
        -o "$object"

	KERNEL_OBJECTS+=("$object")
done < <( \
	find -L src/kernel \
	\( \
		-path 'src/kernel/base/build' -o \
		-path 'src/kernel/base/tests' -o \
		-path 'src/kernel/base/examples' -o \
		-path 'src/kernel/base/src/foundation/diagnostics.c' -o \
		-path 'src/kernel/base/src/foundation/memory/sources/malloc.c' -o \
		-path 'src/kernel/base/src/foundation/memory/sources/cmalloc.c' -o \
		-path 'src/kernel/base/src/foundation/memory/sources/mmap.c' -o \
		-path 'src/kernel/base/src/foundation/memory/pool.c' -o \
		-path 'src/kernel/base/src/foundation/memory/slab.c' -o \
		-path 'src/kernel/base/src/foundation/containers/dynarray.c' -o \
		-path 'src/kernel/base/src/foundation/containers/hashmap.c' \
	\) -prune \
	-o \( \( -type f -o -type l \) -a -name '*.c' \) -print0 \
)

# ===== LINK =====

echo "[5/6] Linking kernel..."

"$LD" \
    -m elf_i386 \
    -T linker.ld \
    -o bin/kernel/kernel.elf \
    bin/kernel/asm/entry.o \
	bin/kernel/asm/exception_handlers.o \
	"${KERNEL_OBJECTS[@]}"

# ===== ELF -> BINARY =====

"$OBJCOPY" \
    -O binary \
    bin/kernel/kernel.elf \
    bin/kernel/kernel.bin

# ===== DISK =====

echo "[6/6] Creating disk image..."

rm -f bin/disks/disk.flp

# Create a 1.44 MiB floppy image
dd if=/dev/zero \
   of=bin/disks/disk.flp \
   bs=512 \
   count=2880 \
   status=none

# Boot sector → sector 1
dd if=bin/bootloader/boot.bin \
   of=bin/disks/disk.flp \
   bs=512 \
   seek=0 \
   conv=notrunc \
   status=none

# Kernel → sector 2 onward
dd if=bin/kernel/kernel.bin \
   of=bin/disks/disk.flp \
   bs=512 \
   seek=1 \
   conv=notrunc \
   status=none

# ===== RUN =====

echo
if $DEBUG; then
    echo "Starting QEMU in debug mode..."
    echo "GDB server: localhost:1234"
    echo "QEMU is paused (-S)."
else
    echo "Starting QEMU..."
fi

QEMU_ARGS=(
    -drive format=raw,unit=0,file=bin/disks/disk.flp
    -name amberOS
    -display gtk
)

if $DEBUG; then
    QEMU_ARGS+=(
        -S
        -gdb tcp::1234
    )
fi

qemu-system-i386 "${QEMU_ARGS[@]}"

#!/bin/bash
set -e

CC=x86_64-elf-gcc
LD=x86_64-elf-ld

CFLAGS="-m32 -ffreestanding -fno-stack-protector -fno-pic -Iinclude"

# Assemble the entry stub + multiboot header
$CC -m32 -ffreestanding -c boot/kernel.s -o boot.o

# Compile the C files
$CC $CFLAGS -c src/kernel.c -o kernel.o
$CC $CFLAGS -c src/vga.c -o vga.o

# Link the kernel as a 32-bit ELF
$LD -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o vga.o

# Prepare GRUB ISO structure
mkdir -p iso/boot/grub
cp kernel.bin iso/boot/

cat > iso/boot/grub/grub.cfg <<EOF
set timeout=0
set default=0

menuentry "SebOS" {
    multiboot /boot/kernel.bin
}
EOF

i686-elf-grub-mkrescue -o SebOS.iso iso

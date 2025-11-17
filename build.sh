#!/bin/bash
set -e
set -x  # <— TEMP: echo commands so we can see what runs

CC=x86_64-elf-gcc
LD=x86_64-elf-ld
CFLAGS="-m32 -ffreestanding -fno-stack-protector -fno-pic -Iinclude"

# Assemble entry + multiboot header
$CC -m32 -ffreestanding -c boot/kernel.s -o boot.o

# Assemble interrupts (ISRs + IRQs)
$CC -m32 -ffreestanding -c boot/interrupts.s -o interrupts.o

$CC -m32 -ffreestanding -c boot/gdt.s -o gdt_asm.o

# Compile C files
$CC $CFLAGS -c src/kernel.c -o kernel.o
$CC $CFLAGS -c src/vga.c    -o vga.o
$CC $CFLAGS -c src/idt.c    -o idt.o
$CC $CFLAGS -c src/isr.c    -o isr.o
$CC $CFLAGS -c src/pic.c    -o pic.o
$CC $CFLAGS -c src/irq.c    -o irq.o
$CC $CFLAGS -c src/pit.c    -o pit.o
$CC $CFLAGS -c src/keyboard.c -o keyboard.o
$CC $CFLAGS -c src/gdt.c    -o gdt.o


# Link kernel
$LD -m elf_i386 -T linker.ld -o kernel.bin \
    boot.o interrupts.o gdt_asm.o \
    kernel.o vga.o idt.o isr.o pic.o irq.o pit.o gdt.o keyboard.o

# Prepare ISO + GRUB
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

/* boot/kernel.s */

/* -------- Multiboot header -------- */
.set MULTIBOOT_MAGIC,  0x1BADB002
.set MULTIBOOT_FLAGS,  0
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

    .section .multiboot
    .align 4
    .long MULTIBOOT_MAGIC
    .long MULTIBOOT_FLAGS
    .long MULTIBOOT_CHECKSUM

/* -------- Code section -------- */
    .section .text
    .global _start
    .extern kernel_main     # kernel_main will be defined in C

_start:
    cli                     # disable interrupts for now

    # Set up stack pointer (esp) to point to stack_top
    lea stack_top, %esp

    # Call into C code
    call kernel_main

.hang:
    hlt                     # halt CPU
    jmp .hang               # if interrupted, halt again

/* -------- Stack memory -------- */
    .section .bss
    .align 16
stack_bottom:
    .skip 16384             # reserve 16 KB for the stack
stack_top:

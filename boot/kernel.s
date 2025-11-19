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
    cli

    # GRUB provides:
    #   eax = multiboot magic
    #   ebx = address of multiboot_info_t
    # We want: kernel_main(uint32_t magic, uint32_t mb_info_addr)

    lea stack_top, %esp      # set up our stack

    push %ebx                # 2nd argument: mb_info_addr
    push %eax                # 1st argument: magic
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

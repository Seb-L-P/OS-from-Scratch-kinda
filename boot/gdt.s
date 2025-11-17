    .section .text

    .global gdt_flush

# void gdt_flush(uint32_t gdt_ptr_addr);
# C passes pointer on stack at 4(%esp)
gdt_flush:
    mov 4(%esp), %eax    # eax = &gdt_ptr
    lgdt (%eax)          # load GDT

    # Load data segment selector (index 2 -> 0x10)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Far jump to flush CS with code segment selector (index 1 -> 0x08)
    ljmp $0x08, $.flush_cs

.flush_cs:
    ret

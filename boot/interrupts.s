    .section .text

    .global idt_flush
    .global isr0
    .global isr1
    .global isr2
    .global isr3
    .global isr4
    .global isr5
    .global isr6
    .global isr7
    .global isr8
    .global isr9
    .global isr10
    .global isr11
    .global isr12
    .global isr13
    .global isr14
    .global isr15
    .global isr16
    .global isr17
    .global isr18
    .global isr19
    .global isr20
    .global isr21
    .global isr22
    .global isr23
    .global isr24
    .global isr25
    .global isr26
    .global isr27
    .global isr28
    .global isr29
    .global isr30
    .global isr31

    .extern isr_handler

# void idt_flush(uint32_t idt_ptr_addr);
idt_flush:
    mov 4(%esp), %eax   # eax = &idt_ptr
    lidt (%eax)         # load IDT
    ret

# Macro-ish pattern: each ISR just pushes its vector number and calls isr_handler

isr0:
    cli
    pushl $0
    call isr_handler
    add $4, %esp
    jmp .hang

isr1:
    cli
    pushl $1
    call isr_handler
    add $4, %esp
    jmp .hang

isr2:
    cli
    pushl $2
    call isr_handler
    add $4, %esp
    jmp .hang

isr3:
    cli
    pushl $3
    call isr_handler
    add $4, %esp
    jmp .hang

isr4:
    cli
    pushl $4
    call isr_handler
    add $4, %esp
    jmp .hang

isr5:
    cli
    pushl $5
    call isr_handler
    add $4, %esp
    jmp .hang

isr6:
    cli
    pushl $6
    call isr_handler
    add $4, %esp
    jmp .hang

isr7:
    cli
    pushl $7
    call isr_handler
    add $4, %esp
    jmp .hang

isr8:
    cli
    pushl $8
    call isr_handler
    add $4, %esp
    jmp .hang

isr9:
    cli
    pushl $9
    call isr_handler
    add $4, %esp
    jmp .hang

isr10:
    cli
    pushl $10
    call isr_handler
    add $4, %esp
    jmp .hang

isr11:
    cli
    pushl $11
    call isr_handler
    add $4, %esp
    jmp .hang

isr12:
    cli
    pushl $12
    call isr_handler
    add $4, %esp
    jmp .hang

isr13:
    cli
    pushl $13
    call isr_handler
    add $4, %esp
    jmp .hang

isr14:
    cli
    pushl $14
    call isr_handler
    add $4, %esp
    jmp .hang

isr15:
    cli
    pushl $15
    call isr_handler
    add $4, %esp
    jmp .hang

isr16:
    cli
    pushl $16
    call isr_handler
    add $4, %esp
    jmp .hang

isr17:
    cli
    pushl $17
    call isr_handler
    add $4, %esp
    jmp .hang

isr18:
    cli
    pushl $18
    call isr_handler
    add $4, %esp
    jmp .hang

isr19:
    cli
    pushl $19
    call isr_handler
    add $4, %esp
    jmp .hang

isr20:
    cli
    pushl $20
    call isr_handler
    add $4, %esp
    jmp .hang

isr21:
    cli
    pushl $21
    call isr_handler
    add $4, %esp
    jmp .hang

isr22:
    cli
    pushl $22
    call isr_handler
    add $4, %esp
    jmp .hang

isr23:
    cli
    pushl $23
    call isr_handler
    add $4, %esp
    jmp .hang

isr24:
    cli
    pushl $24
    call isr_handler
    add $4, %esp
    jmp .hang

isr25:
    cli
    pushl $25
    call isr_handler
    add $4, %esp
    jmp .hang

isr26:
    cli
    pushl $26
    call isr_handler
    add $4, %esp
    jmp .hang

isr27:
    cli
    pushl $27
    call isr_handler
    add $4, %esp
    jmp .hang

isr28:
    cli
    pushl $28
    call isr_handler
    add $4, %esp
    jmp .hang

isr29:
    cli
    pushl $29
    call isr_handler
    add $4, %esp
    jmp .hang

isr30:
    cli
    pushl $30
    call isr_handler
    add $4, %esp
    jmp .hang

isr31:
    cli
    pushl $31
    call isr_handler
    add $4, %esp
    jmp .hang

.hang:
    hlt
    jmp .hang

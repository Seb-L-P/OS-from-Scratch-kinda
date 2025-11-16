#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// An IDT entry (32-bit interrupt gate)
typedef struct {
    uint16_t base_low;   // lower 16 bits of handler address
    uint16_t sel;        // kernel code segment selector
    uint8_t  always0;    // always 0
    uint8_t  flags;      // type and attributes
    uint16_t base_high;  // upper 16 bits of handler address
} __attribute__((packed)) idt_entry_t;

// Pointer passed to lidt
typedef struct {
    uint16_t limit;  // size of IDT - 1
    uint32_t base;   // address of first idt_entry_t
} __attribute__((packed)) idt_ptr_t;

void idt_init(void);

#endif

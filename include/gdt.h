#ifndef GDT_H
#define GDT_H

#include <stdint.h>

//GDT entry (8 bytes)
typedef struct {
    uint16_t limit_low;    // Lower 16 bits of limit
    uint16_t base_low;     // Lower 16 bits of base
    uint8_t  base_middle;  // Next 8 bits of base
    uint8_t  access;       // Access flags
    uint8_t  granularity;  // Granularity + upper 4 bits of limit
    uint8_t  base_high;    // Last 8 bits of base
} __attribute__((packed)) gdt_entry_t;

//GDT pointer (for lgdt)
typedef struct {
    uint16_t limit;        // size of GDT - 1
    uint32_t base;         // Address of first gdt_entry_t
} __attribute__((packed)) gdt_ptr_t;

void gdt_init(void);

#endif
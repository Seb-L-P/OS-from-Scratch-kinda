#include "gdt.h"

// Assembly function to load the GDT and reload segment registers
extern void gdt_flush(uint32_t gdt_ptr_addr);

static gdt_entry_t gdt_entries[3];
static gdt_ptr_t   gdt_ptr;

// Helper to set one GDT entry
static void gdt_set_gate(
    int num,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t gran
) {
    gdt_entries[num].base_low    = (uint16_t)(base & 0xFFFF);
    gdt_entries[num].base_middle = (uint8_t)((base >> 16) & 0xFF);
    gdt_entries[num].base_high   = (uint8_t)((base >> 24) & 0xFF);

    gdt_entries[num].limit_low   = (uint16_t)(limit & 0xFFFF);
    gdt_entries[num].granularity = (uint8_t)((limit >> 16) & 0x0F);

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access       = access;
}

void gdt_init(void) {
    // GDT has 3 entries: null, code, data
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // Null descriptor
    gdt_set_gate(0, 0, 0, 0, 0);

    // Code segment: base=0, limit=4GB, ring 0, code segment
    // Access: 0x9A = 1 0011 010
    //  - present (1)
    //  - ring 0 (00)
    //  - code segment (1)
    //  - executable, readable
    // Granularity: 0xCF
    //  - 4K granularity (G=1)
    //  - 32-bit (D=1)
    //  - limit high nibble = 0xF
    gdt_set_gate(
        1,
        0x00000000,
        0xFFFFFFFF,
        0x9A,
        0xCF
    );

    // Data segment: base=0, limit=4GB, ring 0, data segment
    // Access: 0x92 = 1 0010 010
    gdt_set_gate(
        2,
        0x00000000,
        0xFFFFFFFF,
        0x92,
        0xCF
    );

    // Load new GDT and update segment registers (in assembly)
    gdt_flush((uint32_t)&gdt_ptr);
}

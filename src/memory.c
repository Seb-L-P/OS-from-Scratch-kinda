#include <stdint.h>
#include "memory.h"
#include "multiboot.h"
#include "vga.h"
#include "pmm.h"

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

void memory_init(multiboot_info_t* mbi) {
    if (!(mbi->flags & (1 << 6))) {
        vga_write_line("No memory map provided by GRUB.");
        return;
    }

    vga_write_line("Memory map (from GRUB):");
    vga_write_line("Base          Length        Type");

    multiboot_mmap_entry_t* mmap =
        (multiboot_mmap_entry_t*)mbi->mmap_addr;

    uint32_t mmap_end = mbi->mmap_addr + mbi->mmap_length;

    while ((uint32_t)mmap < mmap_end) {
        vga_write("Base: ");
        vga_write_hex32((uint32_t)mmap->base_addr);
        vga_write("  Len: ");
        vga_write_hex32((uint32_t)mmap->length);
        vga_write("  Type: ");
        vga_write_uint(mmap->type);
        vga_write_line("");

        mmap = (multiboot_mmap_entry_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
    }

    // Now initialise the physical memory manager
    pmm_init(mbi, (uint32_t)&_kernel_start, (uint32_t)&_kernel_end);
}

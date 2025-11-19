#include <stdint.h>
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "memory.h"
#include "multiboot.h"

void kernel_main(uint32_t magic, uint32_t mb_info_addr) {
    vga_init();
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_write_line("Welcome to SebOS!");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    if (magic != 0x2BADB002) {
        vga_write_line("Error: Not booted by a Multiboot-compliant loader.");
        while (1) { __asm__ volatile ("hlt"); }
    }

    multiboot_info_t* mbi = (multiboot_info_t*)mb_info_addr;

    vga_write_line("Initializing GDT...");
    gdt_init();
    vga_write_line("GDT initialized.");

    vga_write_line("Reading memory map...");
    memory_init(mbi);

    vga_write_line("Initializing IDT...");
    idt_init();

    vga_write_line("Remapping PIC and enabling timer & keyboard IRQs...");
    pic_remap(0x20, 0x28);

    vga_write_line("Initializing PIT at 100 Hz...");
    pit_init(100);

    keyboard_init();

    vga_write_line("Enabling interrupts (sti)...");
    __asm__ volatile ("sti");

    vga_write_line("System now handling timer + keyboard interrupts.");
    while (1) {
        __asm__ volatile ("hlt");
    }
}

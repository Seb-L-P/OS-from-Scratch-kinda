#include <stdint.h>
#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "gdt.h"

void kernel_main(void) {
    vga_init();
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_write_line("Custom OS initialised");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    vga_write_line("Initialising GDT...");
    gdt_init();
    vga_write_line("GDT initialised.");

    vga_write_line("Initialising IDT...");
    idt_init();

    vga_write_line("Remapping PIC and enabling timer + keyboard IRQs...");
    pic_remap(0x20, 0x28);

    vga_write_line("Initializing PIT at 100 Hz...");
    pit_init(100);

    keyboard_init();

    vga_write_line("Enabling interrupts (sti)...");
    __asm__ volatile ("sti");

    vga_write_line("System is now receiving timer + keyboard interrupts.");
    vga_write_line("type in QEMU wincdow to see chars appear");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

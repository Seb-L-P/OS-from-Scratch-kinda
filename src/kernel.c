#include <stdint.h>
#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"

void kernel_main(void) {
    vga_init();
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_write_line("im a twat!");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    vga_write_line("Initializing IDT...");
    idt_init();

    vga_write_line("Remapping PIC and enabling timer IRQ...");
    pic_remap(0x20, 0x28);

    vga_write_line("Initializing PIT at 100 Hz...");
    pit_init(100);

    vga_write_line("Enabling interrupts (sti)...");
    __asm__ volatile ("sti");

    vga_write_line("System is now receiving timer interrupts.");
    vga_write_line("Every 100 ticks, a message will be printed.");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

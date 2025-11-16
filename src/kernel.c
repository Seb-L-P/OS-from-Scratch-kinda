#include <stdint.h>
#include "vga.h"

void kernel_main(void) {
    vga_init();

    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_write_line("Welcome to SebOS!");
    vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_WHITE);

    vga_write_line("");
    vga_write_line("u fat twat");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

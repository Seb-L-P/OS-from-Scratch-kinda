#include <stdint.h>
#include "vga.h"
#include "idt.h"

void trigger_divide_by_zero(void) {
    volatile uint32_t a = 1;
    volatile uint32_t b = 0;
    volatile uint32_t c = a / b;
    (void)c;
}

void kernel_main(void) {
    vga_init();
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_write_line("Welcome to SebOS!");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    vga_write_line("Initializing IDT...");
    idt_init();

    vga_write_line("Triggering divide by zero...");
    trigger_divide_by_zero();

    while (1) {
        __asm__ volatile ("hlt");
    }
}

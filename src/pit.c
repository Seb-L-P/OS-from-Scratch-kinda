#include "pit.h"
#include "io.h"
#include "vga.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_BASE_FREQUENCY 1193180

volatile uint32_t pit_ticks = 0;

void pit_init(uint32_t frequency) {
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    // Command byte: channel 0, lobyte/hibyte, mode 3 (square wave)
    outb(PIT_COMMAND, 0x36);

    // Divisor low byte
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    // Divisor high byte
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}

void pit_on_tick(void) {
    pit_ticks++;

    // For demo: print every 100 ticks
    if (pit_ticks % 100 == 0) {
        vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
        vga_write("Ticks: ");
        vga_write_uint(pit_ticks);
        vga_write_line("");
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    }
}

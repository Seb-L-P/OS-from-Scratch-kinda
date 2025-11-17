#include <stdint.h>
#include "keyboard.h"
#include "io.h"
#include "vga.h"

// PS/2 keyboard scancode set 1 (make codes)
static const char scancode_set1[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x0F backspace
    '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',      // 0x1C enter
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\','z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0,   ' ', // 0x39 space
    // more to fill in laterr on
};

void keyboard_init(void) {
    vga_write_line("Keyboard driver initialised. type smth: ");
}

//Called on IRQ1
void keyboard_on_irq(void) {
    uint8_t scancode = inb(0x60);

    // If the top bit is set, it's a key release ("break" code); ignore for now
    if (scancode & 0x80) {
        return;
    }

    // Basic handling for scancode set 1
    char c = 0;
    if (scancode < 128) {
        c = scancode_set1[scancode];
    }

    if (c) {
        vga_put_char(c);
    }
}

#include "vga.h"

static volatile uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;

static int cursor_row = 0;
static int cursor_col = 0;
static uint8_t current_color = (VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void vga_put_entry_at(char c, uint8_t color, int x, int y) {
    const int index = y * VGA_WIDTH + x;
    VGA_BUFFER[index] = vga_entry(c, color);
}

void vga_set_color(vga_color_t fg, vga_color_t bg) {
    current_color = (uint8_t)(fg | (bg << 4));
}

void vga_clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_put_entry_at(' ', current_color, x, y);
        }
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_init(void) {
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_clear();
}

// Scroll everything up by one line
static void vga_scroll(void) {
    // move each row up by one
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int from = y * VGA_WIDTH + x;
            const int to = (y - 1) * VGA_WIDTH + x;
            VGA_BUFFER[to] = VGA_BUFFER[from];
        }
    }
    // clear last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga_put_entry_at(' ', current_color, x, VGA_HEIGHT - 1);
    }
    cursor_row = VGA_HEIGHT - 1;
}

// Handles newlines, scrolling, wrapping
void vga_put_char(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else {
        vga_put_entry_at(c, current_color, cursor_col, cursor_row);
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_HEIGHT) {
        vga_scroll();
    }
}

void vga_write(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        vga_put_char(str[i]);
    }
}

void vga_write_line(const char* str) {
    vga_write(str);
    vga_put_char('\n');
}

void vga_write_uint(uint32_t value) {
    char buf[11]; // up to 10 digits + null
    int i = 0;

    if (value == 0) {
        vga_put_char('0');
        return;
    }

    while (value > 0 && i < 10) {
        uint32_t digit = value % 10;
        buf[i++] = '0' + (char)digit;
        value /= 10;
    }

    // reverse
    for (int j = i - 1; j >= 0; j--) {
        vga_put_char(buf[j]);
    }
}

void vga_write_hex32(uint32_t value) {
    vga_write("0x");
    for (int i = 7; i >= 0; i--) {
        uint8_t nibble = (value >> (i * 4)) & 0xF;
        char c;
        if (nibble < 10) {
            c = '0' + nibble;
        } else {
            c = 'A' + (nibble - 10);
        }
        vga_put_char(c);
    }
}
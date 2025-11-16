#include "vga.h"
#include "isr.h"

// Exception messages for CPU-defined exceptions (0–31)
static const char* exception_messages[32] = {
    "Divide Error",                  // 0
    "Debug",                         // 1
    "Non-maskable Interrupt",        // 2
    "Breakpoint",                    // 3
    "Overflow",                      // 4
    "BOUND Range Exceeded",          // 5
    "Invalid Opcode",                // 6
    "Device Not Available",          // 7
    "Double Fault",                  // 8
    "Coprocessor Segment Overrun",   // 9
    "Invalid TSS",                   // 10
    "Segment Not Present",           // 11
    "Stack Segment Fault",           // 12
    "General Protection Fault",      // 13
    "Page Fault",                    // 14
    "Reserved",                      // 15
    "x87 Floating-Point Error",      // 16
    "Alignment Check",               // 17
    "Machine Check",                 // 18
    "SIMD Floating-Point Exception", // 19
    "Virtualization Exception",      // 20
    "Reserved",                      // 21
    "Reserved",                      // 22
    "Reserved",                      // 23
    "Reserved",                      // 24
    "Reserved",                      // 25
    "Reserved",                      // 26
    "Reserved",                      // 27
    "Reserved",                      // 28
    "Reserved",                      // 29
    "Reserved",                      // 30
    "Reserved"                       // 31
};

void isr_handler(uint8_t int_no) {
    vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    vga_write_line("");
    vga_write_line("=== CPU EXCEPTION ===");

    vga_write("Vector: ");
    vga_write_uint(int_no);

    vga_write(" (");
    if (int_no < 32) {
        vga_write(exception_messages[int_no]);
    } else {
        vga_write("Unknown");
    }
    vga_write_line(")");

    vga_write_line("System halted.");

    // Halt forever
    while (1) {
        __asm__ volatile ("hlt");
    }
}

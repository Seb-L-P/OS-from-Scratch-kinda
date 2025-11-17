#include "pic.h"
#include "io.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT  0x10
#define ICW1_ICW4  0x01
#define ICW4_8086  0x01

void pic_remap(uint8_t offset1, uint8_t offset2) {
    uint8_t a1 = inb(PIC1_DATA); // save masks
    uint8_t a2 = inb(PIC2_DATA);

    // start initialization sequence
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // set new offsets
    outb(PIC1_DATA, offset1); // master vector offset
    outb(PIC2_DATA, offset2); // slave vector offset

    // tell master there is a slave at IRQ2 (0000 0100)
    outb(PIC1_DATA, 0x04);
    // tell slave its cascade identity (2)
    outb(PIC2_DATA, 0x02);

    // 8086/88 (MCS-80/85) mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // For now: enable only IRQ0 (timer) on master, disable all on slave
    outb(PIC1_DATA, 0xFE); // 1111 1110  -> only IRQ0 unmasked
    outb(PIC2_DATA, 0xFF); // all masked
}

void pic_send_eoi(uint8_t irq) {
    // If the IRQ came from the slave PIC, we must acknowledge it there too
    if (irq >= 8) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
}

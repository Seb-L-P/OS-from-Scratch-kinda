#include "irq.h"
#include "vga.h"
#include "pic.h"
#include "pit.h"

void irq_handler(uint8_t irq_no) {
    if (irq_no == 0) {
        pit_on_tick();
    }

    // Tell PIC we handled this interrupt
    pic_send_eoi(irq_no);
}

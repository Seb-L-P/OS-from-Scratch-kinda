#include "irq.h"
#include "vga.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"

void irq_handler(uint8_t irq_no) {
    if (irq_no == 0) {
        //Timer
        pit_on_tick();
    } else if (irq_no == 1) {
        //keyboard
        keyboard_on_irq();
    }

    //PIC informed that we're done
    pic_send_eoi(irq_no);
}

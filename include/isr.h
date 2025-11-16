#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// Common ISR handler for all CPU exceptions (0–31)
void isr_handler(uint8_t int_no);

#endif

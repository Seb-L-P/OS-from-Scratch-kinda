#ifndef PIT_H
#define PIT_H

#include <stdint.h>

void pit_init(uint32_t frequency);
void pit_on_tick(void);

extern volatile uint32_t pit_ticks;

#endif

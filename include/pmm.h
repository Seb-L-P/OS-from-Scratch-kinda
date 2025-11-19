#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include "multiboot.h"

void pmm_init(multiboot_info_t* mbi, uint32_t kernel_start, uint32_t kernel_end);
void* pmm_alloc_frame(void);
void  pmm_free_frame(void* frame);

#endif

#include "pmm.h"
#include "vga.h"

#define PMM_FRAME_SIZE   4096
#define PMM_MAX_MEMORY   (128 * 1024 * 1024)      // 128 MB max
#define PMM_MAX_FRAMES   (PMM_MAX_MEMORY / PMM_FRAME_SIZE)  // 32768
#define PMM_BITMAP_UINTS (PMM_MAX_FRAMES / 32)    // 1024 uint32_t

static uint32_t pmm_bitmap[PMM_BITMAP_UINTS];

static inline void set_frame(uint32_t frame) {
    pmm_bitmap[frame / 32] |=  (1u << (frame % 32));
}

static inline void clear_frame(uint32_t frame) {
    pmm_bitmap[frame / 32] &= ~(1u << (frame % 32));
}

static inline int test_frame(uint32_t frame) {
    return (pmm_bitmap[frame / 32] >> (frame % 32)) & 1u;
}

// Helpers
static uint32_t addr_to_frame(uint32_t addr) {
    return addr / PMM_FRAME_SIZE;
}

void pmm_init(multiboot_info_t* mbi, uint32_t kernel_start, uint32_t kernel_end) {
    // 1. Mark all frames as used initially
    for (uint32_t i = 0; i < PMM_BITMAP_UINTS; i++) {
        pmm_bitmap[i] = 0xFFFFFFFFu;
    }

    // 2. Walk GRUB memory map and mark usable RAM frames as free
    if (mbi->flags & (1 << 6)) {
        multiboot_mmap_entry_t* mmap = (multiboot_mmap_entry_t*)mbi->mmap_addr;
        uint32_t mmap_end = mbi->mmap_addr + mbi->mmap_length;

        while ((uint32_t)mmap < mmap_end) {
            if (mmap->type == 1) { // 1 = usable RAM
                uint32_t base = (uint32_t)mmap->base_addr;
                uint32_t end  = base + (uint32_t)mmap->length;

                // Clip to our max managed range
                if (base >= PMM_MAX_MEMORY) {
                    mmap = (multiboot_mmap_entry_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
                    continue;
                }
                if (end > PMM_MAX_MEMORY) {
                    end = PMM_MAX_MEMORY;
                }

                uint32_t first_frame = (base + PMM_FRAME_SIZE - 1) / PMM_FRAME_SIZE;
                uint32_t last_frame  = end / PMM_FRAME_SIZE;

                for (uint32_t f = first_frame; f < last_frame && f < PMM_MAX_FRAMES; f++) {
                    clear_frame(f);  // mark as free
                }
            }

            mmap = (multiboot_mmap_entry_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
        }
    } else {
        vga_write_line("pmm_init: no memory map from GRUB!");
    }

    // 3. Reserve frames used by the kernel itself (code + data + bss + stack)
    uint32_t k_start_frame = addr_to_frame(kernel_start);
    uint32_t k_end_frame   = (kernel_end + PMM_FRAME_SIZE - 1) / PMM_FRAME_SIZE;

    for (uint32_t f = k_start_frame; f < k_end_frame && f < PMM_MAX_FRAMES; f++) {
        set_frame(f);
    }

    // 4. Optionally reserve very low memory below kernel_start (BIOS area, etc.)
    uint32_t low_frames = k_start_frame;
    for (uint32_t f = 0; f < low_frames && f < PMM_MAX_FRAMES; f++) {
        set_frame(f);
    }

    // 5. Debug: count free frames
    uint32_t free_frames = 0;
    for (uint32_t f = 0; f < PMM_MAX_FRAMES; f++) {
        if (!test_frame(f)) {
            free_frames++;
        }
    }

    vga_write("PMM: managed memory = ");
    vga_write_uint(PMM_MAX_MEMORY / (1024 * 1024));
    vga_write_line(" MB");

    vga_write("PMM: free frames = ");
    vga_write_uint(free_frames);
    vga_write(" (");
    vga_write_uint(free_frames * PMM_FRAME_SIZE / (1024 * 1024));
    vga_write_line(" MB)");
}

// Allocate first free frame (simple but fine for now)
void* pmm_alloc_frame(void) {
    for (uint32_t f = 0; f < PMM_MAX_FRAMES; f++) {
        if (!test_frame(f)) {
            set_frame(f);
            return (void*)(f * PMM_FRAME_SIZE);
        }
    }
    return 0; // no memory
}

void pmm_free_frame(void* frame) {
    if (!frame) return;
    uint32_t addr  = (uint32_t)frame;
    uint32_t index = addr_to_frame(addr);
    if (index < PMM_MAX_FRAMES) {
        clear_frame(index);
    }
}

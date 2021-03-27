
#include <mm.h>


/* pmm_init - initialize the physical memory management */
void page_init(void) {

    int a = 10;
    int b = 20;
    int c = a + b;

    struct e820map *memmap = (struct e820map *)(0x8000);
    uint64_t maxpa = 0;

    // cprintf("e820map:\n");
    int i;
    for (i = 0; i < memmap->nr_map; i ++) {
        uint64_t begin = memmap->map[i].addr, end = memmap->map[i].size;
        end += begin; 
        // uint64_t begin = memmap->map[i].addr, end = begin + memmap->map[i].size;

        //cprintf("  memory: %08llx, [%08llx, %08llx], type = %d.\n",
                // memmap->map[i].size, begin, end - 1, memmap->map[i].type);
        if (memmap->map[i].type == E820_ARM) {
            if (maxpa < end && begin < KMEMSIZE) {
                maxpa = end;
            }
        }
    }
    if (maxpa > KMEMSIZE) {
        maxpa = KMEMSIZE;
    }
}
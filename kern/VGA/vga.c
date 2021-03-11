#include "vga.h"

char *addr = (int *)0x0B8000;

int VGA_Init(){
    for (int i = 0; i < 10; i += 2){
        
        *(addr + i) = 'P';
        *(addr + i + 1) = 0x0c;
    }
}
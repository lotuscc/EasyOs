#include <mm.h>
#include <vga.h>
#include "vmm.h"

void dowhile(){
        /* do nothing */
    while (1);
}

int sub(int a, int b){
    return a - b;
}

int kern_init(void) {

    extern char etext[], edata[], end[];

    int d = sub(20, 10);

    EasyOS_PutIntX((uint32_t)etext, 0, 0);
    EasyOS_PutIntX((uint32_t)edata, 2, 0);
    EasyOS_PutIntX((uint32_t)end, 3, 0);


    EasyOS_ClearScreen();
    // EasyOS_FillScreen('a');
    
    // Page_Init();

    Settest();
    
    OpenPageing();
    
    int e = sub(20, 10);
    
    EasyOS_PutStr("hello", 1, 0);

    dowhile();
}


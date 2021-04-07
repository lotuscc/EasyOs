#include <mm.h>
#include <vga.h>

int kern_init(void) {

    extern char etext[], edata[], end[];


    EasyOS_PutIntX((uint32_t)etext, 0, 0);
    EasyOS_PutIntX((uint32_t)edata, 2, 0);
    EasyOS_PutIntX((uint32_t)end, 3, 0);


    EasyOS_ClearScreen();
    // EasyOS_FillScreen('a');
    
    Page_Init();

    EasyOS_PutStr("hello", 1, 0);

    /* do nothing */
    while (1);
}


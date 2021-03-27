#include <mm.h>
#include <vga.h>

int kern_init(void) {

    int a = 10;
    int b = 20;

    int c = a + b;

    EasyOS_ClearScreen();

    page_init();

    char c1[20] = "peng\n";

    EasyOS_PutStr("lotuscc\n", 0, 0);

    EasyOS_PutStr("lotuscc\nzhangxiang", 0, 2);

    EasyOS_PutStr(c1, 1, 3);

    /* do nothing */
    while (1);
}


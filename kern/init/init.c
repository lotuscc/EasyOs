#include <vga.h>

extern void EasyOS_PutChar(char ch, int pos);


int kern_init(void) {

    int a = 10;
    int b = 20;
    int c = a + b;


    for (int i = 0; i < 20; i++)
    {
        EasyOS_PutChar('s', i);
    }
    

    /* do nothing */
    while (1);
}


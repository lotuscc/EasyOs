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

void EasyOS_PutChar(char ch, int pos){
    char *addr = (unsigned int *)0x0B8000;
    *(addr + pos * 2) = ch;
    *(addr + pos * 2 + 1 ) = 0x0c;
}
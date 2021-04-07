OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

SECTIONS {
    
    /* Boot Entrance Address */
    . = 0x7C00;

    /* TestBoot = .; */

    .startup : {
        *bootasm.o(.text)
    }

    .text : { *(.text) }
    .data : { *(.data .rodata) }
    
    /* Boot End Address */
    /* PROVIDE(BootEnd = .);    */

    /DISCARD/ : { *(.eh_*) }
}

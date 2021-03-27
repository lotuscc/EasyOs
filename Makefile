# define compiler and flags
ASM = gcc
ASMFLAGS = -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -fno-stack-protector -nostdinc

LD = ld
LDFLAGS = -m elf_i386 -nostdlib

CC		= gcc
CFLAGS	= -O0 -fno-builtin -Wall -ggdb -m32 -gstabs -fno-stack-protector -nostdinc

Inc =	-Iboot -Ikern/init/ -Ikern/vga/ -Ikern/mm/

# first task
all: hd60M.img

sign: ./tools/sign.c
	$(CC) -g -Wall -O2 -c ./tools/sign.c -o ./obj/sign.o
	$(CC) -g -Wall -O2 ./obj/sign.o -o bin/sign

boot: sign

	$(ASM) -Iboot $(ASMFLAGS) -Os -c ./boot/bootasm.S -o obj/bootasm.o
	
	$(CC) -Iboot $(CFLAGS) -Os -c ./boot/bootmain.c -o obj/bootmain.o
	
	$(LD) $(LDFLAGS) -N -e start -T tools/boot.ld ./obj/bootasm.o ./obj/bootmain.o -o ./obj/bootblock.o

	objcopy -S -O binary ./obj/bootblock.o ./obj/bootblock.out
	
	./bin/sign ./obj/bootblock.out ./bin/bootblock

kernel:
	$(CC) $(Inc) $(CFLAGS) -c kern/init/init.c -o obj/kern/init.o

	$(CC) $(Inc) $(CFLAGS) -c kern/vga/vga.c -o obj/kern/vga.o

	$(CC) $(Inc) $(CFLAGS) -c kern/mm/mm.c -o obj/kern/mm.o

	$(LD) $(LDFLAGS) -T ./tools/kernel.ld -o ./bin/kernel  \
		./obj/kern/init.o	\
		./obj/kern/vga.o 	\
		./obj/kern/mm.o 

hd60M.img: boot kernel
	dd if=/dev/zero of=./img/hd60M.img bs=1M count=60
	dd if=./bin/bootblock of=./img/hd60M.img bs=512 count=1 conv=notrunc
	dd if=./bin/kernel of=./img/hd60M.img seek=1 conv=notrunc

qemu: hd60M.img 
	qemu-system-i386 -S -s -hda ./img/hd60M.img -monitor stdio

bochs: hd60M.img 
	bochs -f bochsrc


clean:
	rm obj/*.o obj/*.out
	rm obj/kern/*.o
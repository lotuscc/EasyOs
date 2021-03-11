# define compiler and flags
ASM = gcc
ASMFLAGS = -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -fno-stack-protector -nostdinc

LD = ld
LDFLAGS = -m elf_i386 -nostdlib

CC		= gcc
CFLAGS	= -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -fno-stack-protector -nostdinc

# first task
all: c.img

sign: ./tools/sign.c
	$(CC) -g -Wall -O2 -c ./tools/sign.c -o ./obj/sign.o
	$(CC) -g -Wall -O2 ./obj/sign.o -o bin/sign

boot: sign

	$(ASM) -Iboot $(ASMFLAGS) -Os -c ./boot/bootasm.S -o obj/bootasm.o
	
	$(CC) -Iboot $(CFLAGS) -Os -c ./boot/bootmain.c -o obj/bootmain.o
	
	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 ./obj/bootasm.o ./obj/bootmain.o -o ./obj/bootblock.o
	
	objcopy -S -O binary ./obj/bootblock.o ./obj/bootblock.out
	
	./bin/sign ./obj/bootblock.out ./bin/bootblock

kernel:
	$(CC) -Ikern/init/ $(CFLAGS) -c kern/init/init.c -o obj/kern/init/init.o

	$(LD) $(LDFLAGS) -T ./tools/kernel.ld -o ./bin/kernel  ./obj/kern/init/init.o 


c.img: boot kernel
	dd if=/dev/zero of=./img/c.img count=10000
	dd if=./bin/bootblock of=./img/c.img bs=512 count=1 conv=notrunc
	dd if=./bin/kernel of=./img/c.img seek=1 conv=notrunc

qemu: c.img 
	qemu-system-i386 -S -s -hda ./img/c.img -monitor stdio

clean:
	rm obj/*.o
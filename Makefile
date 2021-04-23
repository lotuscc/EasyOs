# define compiler and flags
ASM = gcc
ASMFLAGS = -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -fno-stack-protector -nostdinc

LD = ld
LDFLAGS = -m elf_i386 -nostdlib

CC		= gcc
CFLAGS	= -O0 -fno-builtin -Wall -ggdb -m32 -gstabs -fno-stack-protector -nostdinc

Inc =	-Iboot \
		-Ikern/init/ \
		-Ikern/vga/ \
		-Ikern/mm/ \
		-Ikern/vmm/ \
		-Ikern/driver/ \
		-Ikern/sync/ \
		-Ikern/libs/ 

# first task
all: hd60M.img

sign: ./tools/sign.c
	$(CC) -g -Wall -O2 -c ./tools/sign.c -o ./obj/sign.o
	$(CC) -g -Wall -O2 ./obj/sign.o -o bin/sign

boot: sign

	$(ASM) $(Inc) $(ASMFLAGS) -Os -c ./boot/bootasm.S -o obj/bootasm.o
	
	$(CC) $(Inc) $(CFLAGS) -Os -c ./boot/bootmain.c -o obj/bootmain.o
	
	$(LD) $(LDFLAGS) -N -e start -T tools/boot.ld ./obj/bootasm.o ./obj/bootmain.o -o ./obj/bootblock.o

	objcopy -S -O binary ./obj/bootblock.o ./obj/bootblock.out
	
	./bin/sign ./obj/bootblock.out ./bin/bootblock

kernel:
	$(CC) $(Inc) $(CFLAGS) -c kern/init/init.c -o obj/kern/init.o

	$(CC) $(Inc) $(CFLAGS) -c kern/vga/vga.c -o obj/kern/vga.o

	$(CC) $(Inc) $(CFLAGS) -c kern/vmm/vmm.c -o obj/kern/vmm.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/pic.c -o obj/kern/pic.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/idt.c -o obj/kern/idt.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/timer.c -o obj/kern/timer.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/string.c -o obj/kern/string.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/bitmap.c -o obj/kern/bitmap.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/memory.c -o obj/kern/memory.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/thread.c -o obj/kern/thread.o
	
	$(CC) $(Inc) $(CFLAGS) -c kern/driver/pid.c -o obj/kern/pid.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/intr.c -o obj/kern/intr.o

	$(CC) $(Inc) $(CFLAGS) -c kern/sync/sem.c -o obj/kern/sem.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/console.c -o obj/kern/console.o

	$(CC) $(Inc) $(CFLAGS) -c kern/driver/keyboard.c -o obj/kern/keyboard.o

	$(ASM) $(Inc) $(ASMFLAGS) -c kern/driver/trapentry.S -o obj/kern/trapentry.o

	$(ASM) $(Inc) $(ASMFLAGS) -c kern/driver/vector.S -o obj/kern/vector.o

	$(ASM) $(Inc) $(ASMFLAGS) -c kern/driver/switch.S -o obj/kern/switch.o

	$(ASM) $(Inc) $(ASMFLAGS) -c kern/driver/entry.S -o obj/kern/entry.o

	$(LD) $(LDFLAGS) -T ./tools/kernel.ld -o ./bin/kernel  \
		./obj/kern/init.o	\
		./obj/kern/vga.o 	\
		./obj/kern/vmm.o 	\
		./obj/kern/pic.o 	\
		./obj/kern/idt.o 	\
		./obj/kern/trapentry.o 	\
		./obj/kern/vector.o 	\
		./obj/kern/timer.o 	\
		./obj/kern/string.o 	\
		./obj/kern/bitmap.o 	\
		./obj/kern/thread.o 	\
		./obj/kern/switch.o 	\
		./obj/kern/pid.o 	\
		./obj/kern/entry.o 	\
		./obj/kern/intr.o	\
		./obj/kern/sem.o	\
		./obj/kern/console.o	\
		./obj/kern/keyboard.o	\
		./obj/kern/memory.o 	

hd60M.img: boot kernel
	dd if=./bin/bootblock of=./img/hd60M.img bs=512 count=1 conv=notrunc
	dd if=./bin/kernel of=./img/hd60M.img seek=1 conv=notrunc

qemu: hd60M.img 
	qemu-system-i386 -m 32M -S -s -hda ./img/hd60M.img -monitor stdio

bochs: hd60M.img 
	bochs -f bochsrc


clean:
	rm obj/*.o obj/*.out
	rm obj/kern/*.o
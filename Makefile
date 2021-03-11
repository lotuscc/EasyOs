all: ./tools/sign.c
	gcc -g -Wall -O2 -c ./tools/sign.c -o ./obj/sign.o
	gcc -g -Wall -O2 ./obj/sign.o -o bin/sign

	gcc -Iboot -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -fno-stack-protector \
	-Os -nostdinc -c ./boot/bootasm.S -o obj/bootasm.o
	
	gcc -Iboot -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -fno-stack-protector \
	-Os -nostdinc -c ./boot/bootmain.c -o obj/bootmain.o
	
	ld -m elf_i386 -nostdlib -N -e start -Ttext 0x7C00 ./obj/bootasm.o ./obj/bootmain.o -o ./obj/bootblock.o
	
	objcopy -S -O binary ./obj/bootblock.o ./obj/bootblock.out
	
	./bin/sign ./obj/bootblock.out ./bin/bootblock

	gcc -Ikern/init/ -fno-builtin -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -c kern/init/init.c -o obj/kern/init/init.o

	ld -m elf_i386 -nostdlib -T ./tools/kernel.ld -o ./bin/kernel  ./obj/kern/init/init.o 

c.img: ./bin/bootblock
	dd if=/dev/zero of=./img/c.img count=10000
	dd if=./bin/bootblock of=./img/c.img bs=512 count=1 conv=notrunc
	dd if=./bin/kernel of=./img/c.img seek=1 conv=notrunc
qemu: c.img 
	qemu-system-i386 -S -s -hda ./img/c.img -monitor stdio
all: boot.asm
	nasm boot.asm -o boot.bin

c.img: boot.bin
	#dd if=/dev/zero of=c.img count=10000
	dd if=boot.bin of=c.img bs=512 count=1 conv=notrunc
	#dd if=./bin/kernel of=c.img seek=1 conv=notrunc
qemu: c.img 
	qemu-system-i386 -hda c.img

bochs: c.img
	bochs -f bochsrc.bxrc
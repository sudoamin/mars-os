
bootloader.o:
	nasm -f bin boot_loader.asm -o boot.bin

image:
	dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc

run:
	qemu-system-x86_64 boot.img

clean:
	rm boot.bin
	rm boot.img
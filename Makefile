all: boot.bin

boot.bin:
	nasm -f bin bootloader/bootloader.asm -o build/boot.bin

image:
	dd if=build/boot.bin of=build/boot.img bs=512 count=1 conv=notrunc

run:
	qemu-system-x86_64 build/boot.img

text:
	echo "Hello World" >> build/msg.txt
	dd if=build/msg.txt >> build/boot.img
	dd if=/dev/zero bs=512 count=1 >> build/boot.img

clean:
	rm -rf build/*
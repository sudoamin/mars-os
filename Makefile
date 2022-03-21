all: image

boot.bin:
	nasm -f bin bootloader/bootloader.asm -o build/boot.bin

kernel.bin:
	nasm -f bin kernel/kernel.asm -o build/kernel.bin

image: boot.bin kernel.bin
	dd if=build/boot.bin of=build/mars.img bs=512 count=1 conv=notrunc
	dd if=build/kernel.bin >> build/mars.img
	dd if=/dev/zero bs=512 count=100 >> build/mars.img

run:
	qemu-system-x86_64 build/mars.img

text:
	echo "Hello World" >> build/msg.txt
	dd if=build/msg.txt >> build/mars.img
	dd if=/dev/zero bs=512 count=1 >> build/mars.img

clean:
	rm -rf build/*
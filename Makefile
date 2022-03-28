all: image

boot.bin:
	nasm -f bin bootloader/bootloader.asm -o build/boot.bin

kernel.bin:
	nasm -f elf64 kernel/int/idt.asm -o build/idt.s.o
	gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c kernel/int/idt.c -o build/idt.o
	gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c kernel/int/int.c -o build/int.o

	gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c lib/string.c -o build/string.o
	gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c lib/debug.c -o build/debug.o
	nasm -f elf64 lib/mem.asm -o build/mem.s.o

	gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c console/print.c -o build/print.o
	gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c console/console.c -o build/console.o

	nasm -f elf64 kernel/kernel.asm -o build/kernel.s.o
	gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c kernel/kernel.c -o build/kernel.o

	ld -nostdlib -T kernel/linker.ld -o build/kernel.elf build/kernel.s.o build/kernel.o \
	build/idt.s.o build/idt.o build/int.o build/debug.o build/console.o build/string.o \
	build/mem.s.o build/print.o
	
	objcopy -O binary build/kernel.elf build/kernel.bin

image: boot.bin kernel.bin
	dd if=build/boot.bin of=build/mars.img bs=512 count=1 conv=notrunc
	dd if=build/kernel.bin >> build/mars.img
	dd if=/dev/zero bs=512 count=100 >> build/mars.img

run:
	qemu-system-x86_64 build/mars.img

clean:
	rm -rf build/*
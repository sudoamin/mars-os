CC = gcc
C_FLAGS = -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone
K_INCLUDE = -I. -Ikernel/include -Iinclude -Iusr/include
U_INCLUDE = -I. -Iinclude -Iusr/include
OBJECTS = build/kernel.s.o build/kernel.o \
	build/idt.s.o build/idt.o build/int.o \
	build/mem.o build/mem.s.o build/proc.o build/syscall.o \
	build/kconsole.o build/ps2.o

all: image
.PHONY: bootloader kernel

fast: image run clean

bootloader:
	nasm -f bin bootloader/boot.asm -o build/boot.bin
	nasm -f bin bootloader/loader.asm -o build/loader.bin

kernel: lib.a
	nasm -f elf64 kernel/int/idt.asm -o build/idt.s.o
	${CC} ${C_FLAGS} ${K_INCLUDE} -c kernel/int/idt.c -o build/idt.o
	${CC} ${C_FLAGS} ${K_INCLUDE} -c kernel/int/int.c -o build/int.o

	${CC} ${C_FLAGS} ${K_INCLUDE} -c kernel/console/console.c -o build/kconsole.o

	nasm -f elf64 kernel/kernel.asm -o build/kernel.s.o
	nasm -f elf64 kernel/mem/mem.asm -o build/mem.s.o
	${CC} ${C_FLAGS} ${K_INCLUDE} -c kernel/kernel.c -o build/kernel.o
	${CC} ${C_FLAGS} ${K_INCLUDE} -c kernel/mem/mem.c -o build/mem.o
	${CC} ${C_FLAGS} ${K_INCLUDE} -c kernel/proc/proc.c -o build/proc.o
	${CC} ${C_FLAGS} ${K_INCLUDE} -c kernel/syscall/syscall.c -o build/syscall.o

	${CC} ${C_FLAGS} ${K_INCLUDE} -c drivers/ps2/ps2.c -o build/ps2.o

	ld -nostdlib -T linker.ld -o build/kernel.elf ${OBJECTS} build/lib.a
	
	objcopy -O binary build/kernel.elf build/kernel.bin

lib.a:
	${CC} ${C_FLAGS} ${U_INCLUDE} -c lib/string/string.c -o build/string.o
	${CC} ${C_FLAGS} ${U_INCLUDE} -c lib/debug/debug.c -o build/debug.o
	${CC} ${C_FLAGS} ${U_INCLUDE} -c lib/console/console.c -o build/console.o
	nasm -f elf64 lib/string/string.asm -o build/string.s.o
	nasm -f elf64 lib/syscall/syscall.asm -o build/syscall.s.o
	
	ar -rcs build/lib.a build/string.s.o build/syscall.s.o build/string.o build/debug.o \
	build/console.o

ps: image
	nasm -f elf64 usr/src/ps1/execute.asm -o usr/src/ps1/build/execute.s.o
	${CC} ${C_FLAGS} ${U_INCLUDE} -c usr/src/ps1/main.c -o usr/src/ps1/build/main.o
	ld -nostdlib -Tusr/src/ps1/linker.ld -o usr/src/ps1/build/ps1 usr/src/ps1/build/main.o usr/src/ps1/build/execute.s.o build/lib.a
	objcopy -O binary usr/src/ps1/build/ps1 usr/src/ps1/build/ps1.bin
	dd if=usr/src/ps1/build/ps1.bin of=build/mars.img bs=512 count=10 seek=100 conv=notrunc

	nasm -f elf64 usr/src/rover/execute.asm -o usr/src/rover/build/execute.s.o
	${CC} ${C_FLAGS} ${U_INCLUDE} -c usr/src/rover/main.c -o usr/src/rover/build/main.o
	ld -nostdlib -Tusr/src/rover/linker.ld -o usr/src/rover/build/rover usr/src/rover/build/main.o usr/src/rover/build/execute.s.o build/lib.a
	objcopy -O binary usr/src/rover/build/rover usr/src/rover/build/rover.bin
	dd if=usr/src/rover/build/rover.bin of=build/mars.img bs=512 count=10 seek=110 conv=notrunc

ps_clean: clean
	rm -rf usr/src/ps1/build/*
	rm -rf usr/src/rover/build/*

image: bootloader kernel
	dd if=build/boot.bin of=build/mars.img bs=512 count=1 conv=notrunc
	dd if=build/loader.bin of=build/mars.img bs=512 count=10 seek=1 conv=notrunc
	
	dd if=build/kernel.bin of=build/mars.img bs=512 count=100 seek=8 conv=notrunc
	dd if=/dev/zero bs=512 count=100 >> build/mars.img 

run:
	qemu-system-x86_64 -m 1024 build/mars.img

clean:
	rm -rf build/*
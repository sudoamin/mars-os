CC=gcc
TARGET=mars
C_FILES=$(shell find . -type f -iname '*.c')
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
OBJS=$(C_FILES:.c=.o)

all compile: $(TARGET)
all: iso run clean
.PHONY: all compile clean iso 

%.o:
	$(CC) -c $(@:.o=.c) -o $@ -ffreestanding -fno-exceptions -m32

$(TARGET): $(OBJS)
	$(shell nasm -f elf src/boot/loader.asm -o loader.s.o)
	$(shell nasm -f elf src/idt/idt.asm -o idt.s.o)
	$(CC) -m32 $(FLAGS) -lgcc loader.s.o idt.s.o $? -T src/linker.ld -o $(TARGET)
	mv mars iso/boot

iso:
	$(shell grub-mkrescue iso --output=$(TARGET).iso)

run:
	qemu-system-x86_64 -cdrom $(TARGET).iso

clean:
	rm $(shell find . -type f -iname '*.o')
	rm $(TARGET).iso
CC=gcc
TARGET=mars
C_FILES=$(shell find . -type f -iname '*.c')
OBJS=$(C_FILES:.c=.o)

all compile: $(TARGET)
all: iso run clean
.PHONY: all compile clean iso 

%.o:
	$(CC) -c $(@:.o=.c) -o $@ -ffreestanding -fno-exceptions -m32

$(TARGET): $(OBJS)
	$(shell nasm -f elf src/boot/boot.asm -o boot.o)
	$(CC) -m32 -nostdlib -nodefaultlibs -lgcc boot.o $? -T src/linker.ld -o $(TARGET)
	mv mars iso/boot

iso:
	$(shell grub-mkrescue iso --output=$(TARGET).iso)

run:
	qemu-system-x86_64 -cdrom mars.iso

clean:
	rm src/kernel/kernel.o
	rm iso/boot/mars
	rm boot.o
	rm mars.iso
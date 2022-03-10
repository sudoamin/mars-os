CC=gcc
TARGET=mars
C_FILES=$(shell find . -type f -iname '*.c')
OBJS=$(C_FILES:.c=.o)

all compile: $(TARGET)
all: iso
.PHONY: all compile clean iso

%.o:
	$(CC) -c $(@:.o=.c) -o $@ -ffreestanding -fno-exceptions -m32

$(TARGET): $(OBJS)
	$(shell nasm -f elf src/boot/boot.asm -o boot.o)
	$(CC) -m32 -nostdlib -nodefaultlibs -lgcc boot.o $? -T linker.ld -o $(TARGET)
	mv mars iso/boot

iso:
	$(shell grub-mkrescue iso --output=$(TARGET).iso)

run:
	qemu-system-x86_64 -cdrom mars.iso

clean:
	$(shell find . -name \*.o | xargs --no-run-if-empty rm)
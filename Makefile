CC=gcc
TARGET=mars
C_FILES=./kernel.c
OBJS=$(C_FILES:.c=.o)

all compile: $(TARGET)
all: finale
.PHONY: all compile clean finale

%.o:
	$(CC) -c $(@:.o=.c) -o $@ -ffreestanding -fno-exceptions -m32

$(TARGET): $(OBJS)
	$(shell nasm -f elf boot.asm -o boot.o)
	$(CC) -m32 -nostdlib -nodefaultlibs -lgcc boot.o $? -T linker.ld -o $(TARGET)


finale:
    $(shell cd ~/clang/mars-os/)
    $(shell cp $(TARGET) ./iso/boot/$(TARGET))
    $(shell grub-mkrescue iso --output=$(TARGET).iso)

clean:
    $(shell rm -f *.o $(TARGET) $(TARGET).iso)
    $(shell find . -name \*.o | xargs --no-run-if-empty rm)
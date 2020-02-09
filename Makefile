CC=gcc
CFLAGS=-std=gnu89 -m32 -Wall -Wextra -Wno-unused-parameter
CFLAGS+=-fno-builtin -nostdlib -ffreestanding -fno-stack-protector
LDFLAGS=-melf_i386 -T link.ld

.PHONY: all disk run clean disk-clean
all: boot.bin io.sys

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

io.elf: io.c.o
	$(LD) $(LDFLAGS) -o $@ $^

io.sys: io.elf
	objcopy -O binary io.elf io.sys

boot.bin: boot.asm
	nasm -f bin -o $@ $^

disk-clean:
	rm -f floppy.img

disk: disk-clean all
	mkfs.msdos -C floppy.img 1440
	dd if=boot.bin of=floppy.img bs=512 count=1 conv=notrunc
	dd if=io.sys of=floppy.img seek=1 bs=512 conv=notrunc

run: disk
	qemu-system-i386 -fda floppy.img -boot a -soundhw pcspk

clean: disk-clean
	rm -f *.o io.elf io.sys boot.bin

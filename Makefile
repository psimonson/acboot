CC=gcc
CFLAGS=-std=gnu89 -Wall -Wextra -Wno-unused-parameter
LDFLAGS=
KERNEL=no
ifeq ($(KERNEL),yes)
CFLAGS+=-m32 -fno-builtin -nostdlib -ffreestanding -fno-stack-protector
LDFLAGS=-melf_i386 -T link.ld
endif

.PHONY: all disk kernel run clean disk-clean
ifeq ($(KERNEL),yes)
all: boot.bin io.sys
else
all: ft
endif

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

io.elf: io.c.o
	$(LD) $(LDFLAGS) -o $@ $^

io.sys: io.elf
	objcopy -O binary io.elf io.sys

boot.bin: boot.asm
	nasm -f bin -o $@ $^

ft: ft.c.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

kernel:
	$(MAKE) KERNEL=yes
	$(MAKE)

disk-clean:
	rm -f floppy.img

disk: disk-clean kernel
#	mkfs.msdos -C floppy.img 1440
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	./ft
#	dd if=boot.bin of=floppy.img bs=512 count=1 conv=notrunc
#	dd if=io.sys of=floppy.img seek=1 bs=512 conv=notrunc

run: disk
	qemu-system-i386 -fda floppy.img -boot a -soundhw pcspk

clean: disk-clean
	rm -f *.o ftable.dat ft io.elf io.sys boot.bin

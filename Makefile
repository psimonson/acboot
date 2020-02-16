CC=gcc
CFLAGS=-std=gnu89 -Wall -Wextra -Wno-unused-parameter
LDFLAGS=
KERNEL=no
ifeq ($(KERNEL),yes)
CFLAGS+=-m32 -fno-builtin -nostdlib -ffreestanding -fno-stack-protector
LDFLAGS=-m elf_i386
endif

.PHONY: all disk kernel run clean disk-clean
ifeq ($(KERNEL),yes)
all: boot.bin IO.SYS SHELL.APP
else
all: ft
endif

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

io.elf: io.c.o stdio.c.o disk.c.o fs.c.o
	$(LD) $(LDFLAGS) -T link.ld -o $@ $^

shell.elf: binary.c.o stdio.c.o disk.c.o fs.c.o
	$(LD) $(LDFLAGS) -T binary.ld -o $@ $^

IO.SYS: io.elf
	objcopy -O binary $^ $@

SHELL.APP: shell.elf
	objcopy -O binary $^ $@

boot.bin: boot.asm
	nasm -f bin -o $@ $^

ft: ft.c.o prsfs.c.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

kernel:
	$(MAKE) KERNEL=yes
	$(MAKE)

disk-clean:
	rm -f floppy.img

disk: disk-clean kernel
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	./ft

run: disk
	qemu-system-i386 -fda floppy.img -boot a -soundhw pcspk

clean: disk-clean
	rm -f *.o ft shell.elf io.elf SHELL.APP IO.SYS boot.bin

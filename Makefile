CC=gcc
CFLAGS=-std=gnu89 -Wall -Wextra -Wno-unused-parameter
LDFLAGS=
KERNEL=no
ifeq ($(KERNEL),yes)
CFLAGS+=-m16 -fno-builtin -nostdlib -ffreestanding -fno-stack-protector
LDFLAGS=-m elf_i386
DEBUG?=no
ifeq ($(DEBUG),yes)
DEBUG=--strip-debug
else
DEBUG=
endif
endif

.PHONY: all disk kernel run clean disk-clean
ifeq ($(KERNEL),yes)
all: boot.bin IO.SYS SHELL.APP GRAPH.APP HELLO.APP
else
all: ft
	$(MAKE) KERNEL=yes
endif

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

io.elf: io.c.o stdio.c.o disk.c.o fs.c.o
	$(LD) $(LDFLAGS) -T io.ld -o $@ $^

shell.elf: shell.c.o stdio.c.o disk.c.o fs.c.o
	$(LD) $(LDFLAGS) -T shell.ld -o $@ $^

graph.elf: graph.c.o stdio.c.o disk.c.o
	$(LD) $(LDFLAGS) -T link.ld -o $@ $^

hello.elf: hello.c.o stdio.c.o disk.c.o
	$(LD) $(LDFLAGS) -T link.ld -o $@ $^

IO.SYS: io.elf
	objcopy $(DEBUG_INFO) -O binary $^ $@

SHELL.APP: shell.elf
	objcopy $(DEBUG_INFO) -O binary $^ $@

GRAPH.APP: graph.elf
	objcopy $(DEBUG_INFO) -O binary $^ $@

HELLO.APP: hello.elf
	objcopy $(DEBUG_INFO) -O binary $^ $@

boot.bin: boot.asm
	nasm -f bin -o $@ $^

ft: ft.c.o prsfs.c.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

kernel:
	$(MAKE) KERNEL=yes
	$(MAKE)

kernel-debug:
	$(MAKE) KERNEL=yes DEBUG=yes
	$(MAKE)

disk-clean:
	rm -f floppy.img

disk-debug: disk-clean kernel-debug
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	./ft

disk: disk-clean kernel
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	./ft

run-debug: disk-debug
	qemu-system-i386 -fda floppy.img -boot a -soundhw pcspk -s -S

run: disk
	qemu-system-i386 -fda floppy.img -boot a -soundhw pcspk

clean: disk-clean
	rm -f *.o ft *.elf *.APP IO.SYS boot.bin

CC=gcc
CFLAGS=-std=gnu89 -Wall -Wextra -Wno-unused-parameter
LDFLAGS=
KERNEL=no
ifeq ($(KERNEL),yes)
CFLAGS+=-m32 -fno-builtin -nostdlib -ffreestanding -fno-stack-protector
LDFLAGS=-m elf_i386 -no-startfiles --nmagic
DEBUG?=no
ifeq ($(DEBUG),yes)
STRIP_ARGS=--strip-debug
else
STRIP_ARGS=--strip-debug --strip-dwo
endif
endif

.PHONY: all disk kernel run clean disk-clean
ifeq ($(KERNEL),yes)
all: boot.bin IO.SYS SHELL.APP GRAPH.APP
else
all: kernel
endif

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
ifeq ($(DEBUG),no)
	strip $(STRIP_ARGS) $@
endif

io.elf: io.c.o stdio.c.o disk.c.o fs.c.o
	$(LD) $(LDFLAGS) -T io.ld -o $@ $^
ifeq ($(DEBUG),no)
	strip $(STRIP_ARGS) $@
endif

shell.elf: shell.c.o stdio.c.o system.c.o disk.c.o fs.c.o
	$(LD) $(LDFLAGS) -T shell.ld -o $@ $^
ifeq ($(DEBUG),no)
	strip $(STRIP_ARGS) $@
endif

graph.elf: graph.c.o stdio.c.o special.c.o system.c.o disk.c.o
	$(LD) $(LDFLAGS) -T link.ld -o $@ $^
ifeq ($(DEBUG),no)
	strip $(STRIP_ARGS) $@
endif

IO.SYS: io.elf
	objcopy -O binary $^ $@

SHELL.APP: shell.elf
	objcopy -O binary $^ $@

GRAPH.APP: graph.elf
	objcopy -O binary $^ $@

boot.bin: boot.asm
	nasm -f bin -o $@ $^

ft: ft.c.o prsfs.c.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

kernel:
	$(MAKE) KERNEL=yes DEBUG=no
	$(MAKE) ft

kernel-debug:
	$(MAKE) KERNEL=yes DEBUG=yes
	$(MAKE) ft

disk-clean:
	rm -f floppy.img

disk-debug: clean kernel-debug
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	./ft

disk: clean kernel
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	./ft

run-debug: disk-debug
	qemu-system-i386 -fda floppy.img -boot a -soundhw pcspk -s -S

run: disk
	qemu-system-i386 -fda floppy.img -boot a -soundhw pcspk

clean: disk-clean
	rm -f *.o ft *.elf *.APP IO.SYS boot.bin

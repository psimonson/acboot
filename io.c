/**
 * @file io.c
 * @author Philip R. Simonson
 * @date 02/07/2020
 * @brief Simple kernel for my boot loader.
 ***************************************************************
 */

asm(".code16gcc");
asm("jmpl $0, $main");

#include "stdio.h"
#include "types.h"
#include "disk.h"
#include "attr.h"
	unsigned char sector[BLOCK_SIZE];

/* Entry point for boot program.
 */
void main(void)
{
	unsigned char drive = -1;
	drive_params_t p;
	char buf[32];
	int i;

	asm volatile("" : "=d"(drive));
	get_drive_params(drive, &p);
	printf("BIOS drive: %d\r\n", p.drive);
	if(read_disk(sector, &p, 1, 1)) {
		printf("FILE: ");
		for(i = 0; i < 11; i++)
			putc(((struct file*)sector)->filename[i]);
		printf("\r\n");
	}
	printf("Please enter your name: ");
	gets(buf, sizeof(buf));
	printf("Hello, %s!\r\n", buf);
	type("Press any key to reboot...\r\n");

	i = 0;
	while(kbhit() == 0) {
		printf("Hello world: %d\r\n", i);
		timer(0x0007, 0x05f0);
		i++;
	}
	asm volatile(
		"jmpl $0xffff, $0x0000"
	);
}

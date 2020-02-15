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

/* Entry point for boot program.
 */
void main(void)
{
	unsigned char drive = -1;
	unsigned char *table;
	drive_params_t p;
	char buf[32];
	int i;

	asm volatile("" : "=d"(drive));
	setup();
	get_drive_params(drive, &p);
	printf("BIOS drive: %d\r\n", p.drive);
	if((table = get_ftable(&p)) != NULL) {
		for(i = 0; i < 2; i++) {
			void *ftable = &table[i*16];
			int j;

			printf("FILE: ");
			for(j = 0; j < 11; j++)
				putc(((struct file*)ftable)->filename[j]);
			printf("\r\nSector Count: %d\r\nStarting Sector: %d\r\n",
				((struct file*)ftable)->num_sectors,
				((struct file*)ftable)->start);
		}
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

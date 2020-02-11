/**
 * @file io.c
 * @author Philip R. Simonson
 * @date 02/07/2020
 * @brief Simple kernel for my boot loader.
 ***************************************************************
 */

asm(".code16gcc");
asm("jmpl $0x0000, $main");

#include "stdio.h"
#include "attr.h"

unsigned char bios_drive = -1;
/* Entry point for boot program.
 */
void main(void)
{
	char buf[32];
	int i;

	unsigned short drive = -1;
	asm volatile("" : "=d"(drive));
	bios_drive = drive & 0x00ff;
	printf("Please enter your name: ");
	gets(buf, sizeof(buf));
	printf("Hello, %s!\r\n", buf);
	printf("BIOS drive: %d\r\n", bios_drive);
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

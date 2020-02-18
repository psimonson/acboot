/**
 * @file hello.c
 * @author Philip R. Simonson
 * @date 02/17/2020
 * @brief Simple hello world app for my operating system.
 *********************************************************************
 */

asm(".code16gcc");
asm("jmpl $0, $main");

#include "stdio.h"
#include "disk.h"

/* Boot sector entry */
#define BOOT_ENTRY 0x0500

/* Simple shell program for my OS.
 */
void main(void)
{
	const void *e = (const void *)BOOT_ENTRY;
	unsigned char drive = -1;
	drive_params_t p;

	asm volatile("" : "=d"(drive));

	setup(0x0000);
	get_drive_params(drive, &p);

	printf("Hello world!\r\n");

	asm volatile("" : : "d"(p.drive));
	goto *e;
}

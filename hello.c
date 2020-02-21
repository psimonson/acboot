/**
 * @file hello.c
 * @author Philip R. Simonson
 * @date 02/17/2020
 * @brief Simple hello world app for my operating system.
 *********************************************************************
 */

asm(".code16gcc");
asm("jmp main");

#include "stdio.h"
#include "disk.h"

#define SHELL_ENTRY 0x0800

/* Simple shell program for my OS.
 */
void main(void)
{
	const void *e = (const void *)SHELL_ENTRY;
	unsigned char drive = -1;
	drive_params_t p;

	setup();
	asm volatile("" : "=d"(drive));
	get_drive_params(drive, &p);

	printf("Hello world!\r\n");

	asm volatile("" : : "d"(p.drive));
	goto *e;
}

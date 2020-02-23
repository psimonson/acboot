/**
 * @file io.c
 * @author Philip R. Simonson
 * @date 02/07/2020
 * @brief Simple kernel for my boot loader.
 ***************************************************************
 */

asm(".code16gcc");
asm("jmp main");

#include "stdio.h"
#include "types.h"
#include "disk.h"
#include "fs.h"

/* Entry point for boot program.
 */
void main(void)
{
	unsigned char drive = -1;
	unsigned char *table = NULL;
	struct file *entry = NULL;
	drive_params_t p;

	asm volatile(
		"movw %%cs, %%ax\n"
		"movw %%ax, %%ds\n"
		"movw %%ax, %%es\n"
		"movw %%ax, %%ss\n"
		"movb %%dl, %0\n"
		: "=r"(drive)
	);
	get_drive_params(drive, &p);

	printf("BIOS drive: %d\r\n", p.drive);
	if((table = get_ftable(&p)) != NULL) {
		const char *filename = get_filename_user("SHELL.APP");
		if((entry = search_file(table, filename)) != NULL)
			exec_file(&p, entry);
		else
			printf("File: %s Not found.\r\n", filename);
	}

	type("Press any key to reboot...\r\n");
	asm volatile(
		"xorw %ax, %ax\n"
		"int $0x16\n"
		"jmpl $0xffff, $0x0000"
	);
}

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
#include "fs.h"

#define SHELL_ENTRY 0x7e00

/* Execute command shell program.
 */
void exec_shell(const drive_params_t *p, const struct file *e)
{
	void *buffer = (void *)SHELL_ENTRY;
	unsigned short nread = 0;

	reset_disk(p);
	if((nread = read_disk(buffer, p, e->start, e->count)) == e->count) {
		asm volatile("movb %0, %%dl" : : "r"(p->drive));
		goto *buffer;
	}
	printf("Failed to execute binary.\r\n");
}
/* Entry point for boot program.
 */
void main(void)
{
	unsigned char drive = -1;
	unsigned char *table;
	struct file *entry;
	drive_params_t p;

	setup();
	asm volatile("" : "=d"(drive));
	get_drive_params(drive, &p);
	printf("BIOS drive: %d\r\n", p.drive);
	if((table = get_ftable(&p)) != NULL) {
		if((entry = search_file(table, "SHELL.APP")) != NULL)
			exec_shell(&p, entry);
		else
			printf("File: %s Not found.\r\n");
	}
	type("Press any key to reboot...\r\n");
	asm volatile(
		"jmpl $0x0000, $0xffff"
	);
}

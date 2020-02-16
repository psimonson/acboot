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

/* Entry point for boot program.
 */
void main(void)
{
	unsigned char drive = -1;
	unsigned char *table;
	struct file *entry;
	drive_params_t p;
	char buf[32];
	int i;

	asm volatile("" : "=d"(drive));
	setup();
	get_drive_params(drive, &p);
	printf("BIOS drive: %d\r\n", p.drive);
	if((table = get_ftable(&p)) != NULL) {
		printf("Enter filename to execute: ");
		gets(buf, sizeof(buf));
		if((entry = search_file(table, buf)) != NULL)
			exec_file(&p, entry);
		else
			printf("File: %s Not found.\r\n");
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

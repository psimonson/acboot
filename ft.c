/**
 * @file ft.c
 * @author Philip R. Simonson
 * @date 02/08/2020
 * @brief Simple file system for floppy.
 *************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "prsfs.h"

#define START_SECTOR  2		/* starting sector of operating system */
#define TOTAL_SECTORS 8		/* total operating system sectors */

/* Program to create a simple file system.
 */
int main(void)
{
	struct file ftable[MAXFILES];
	int fout;

	init_table(ftable);
	init_entry(&ftable[0], "IO      SYS", TOTAL_SECTORS, START_SECTOR);
	errno = 0;
	if((fout = open("floppy.img", O_RDWR | O_CREAT)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return 1;
	}
	write_file(fout, 0, 1, "boot.bin");
	write_table(fout, ftable);
	write_file(fout, START_SECTOR, TOTAL_SECTORS, "io.sys");
	close(fout);
	return 0;
}

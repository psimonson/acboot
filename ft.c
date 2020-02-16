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

/* number of files to write */
#define FILE_COUNT	2
/* some defines for io sys */
#define IO_START	2		/* starting sector of operating system */
#define IO_SECTORS	10		/* total operating system sectors */
/* some defines for binary app */
#define BINARY_START (IO_START+(IO_SECTORS+1))
#define BINARY_SECTORS 11
/* Program to create a simple file system.
 */
int main(void)
{
	struct file ftable[MAXFILES];
	int fout;

	init_table(ftable);
	init_entry(&ftable[0], "IO      SYS", IO_SECTORS, IO_START);
	init_entry(&ftable[1], "BINARY  APP", BINARY_SECTORS, BINARY_START);
	errno = 0;
	if((fout = open("floppy.img", O_RDWR | O_CREAT)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return 1;
	}
	write_file(fout, 0, 1, "boot.bin");
	write_table(fout, ftable, FILE_COUNT);
	close(fout);
	return 0;
}

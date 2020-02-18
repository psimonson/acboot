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

/* some defines for io sys */
#define IO_START	2		/* starting sector of operating system */
#define IO_SECTORS	14		/* total operating system sectors */
/* some defines for binary app */
#define SHELL_START (IO_START+IO_SECTORS)+1
#define SHELL_SECTORS 17
/* some defines for graph app */
#define GRAPH_START (SHELL_START+SHELL_SECTORS)+1
#define GRAPH_SECTORS 11
/* some defines for hello app */
#define HELLO_START (GRAPH_START+GRAPH_SECTORS)+1
#define HELLO_SECTORS 10

/* Program to create a simple file system.
 */
int main(void)
{
	struct file ftable[MAXFILES];
	int fout;

	init_table(ftable);
	init_entry(&ftable[0], "IO      SYS", IO_SECTORS, IO_START);
	init_entry(&ftable[1], "SHELL   APP", SHELL_SECTORS, SHELL_START);
	init_entry(&ftable[2], "GRAPH   APP", GRAPH_SECTORS, GRAPH_START);
	init_entry(&ftable[3], "HELLO   APP", HELLO_SECTORS, HELLO_START);
	errno = 0;
	if((fout = open("floppy.img", O_RDWR | O_CREAT)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return 1;
	}
	write_file(fout, 0, 1, "boot.bin");
	write_table(fout, ftable);
	close(fout);
	return 0;
}

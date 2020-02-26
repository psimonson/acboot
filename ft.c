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
#include "file.h"

/* Program to create a simple file system.
 */
int main(void)
{
	struct file ftable[MAXFILES];
	int fout;

	init_table(ftable);
	init_entry(&ftable[0], "IO      SYS");
	init_entry(&ftable[1], "SHELL   APP");
	init_entry(&ftable[2], "GRAPH   APP");
/*	init_entry(&ftable[3], "HELLO   APP");*/
	errno = 0;
	if((fout = open("floppy.img", O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return 1;
	}
	write_file(fout, 0, 1, "boot.bin");
	write_table(fout, ftable, IO_START);
	close(fout);
	return 0;
}

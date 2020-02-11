/**
 * @file prsfs.c
 * @author Philip R. Simonson
 * @date 02/11/2020
 * @brief Simple file system source code for floppy.
 *************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "prsfs.h"

/* Initialise ftable memory.
 */
void init_table(struct file *table)
{
	const unsigned char c = 0xf7;
	int i;
	memset(table, 0, sizeof(struct file)*MAXFILES);
	for(i = 0; i < MAXFILES; i++)
		memcpy(table[i].filename, &c, 1);
}
/* Initialise ftable entry.
 */
void init_entry(struct file *entry, const char *filename,
	unsigned short num_sectors, unsigned char start)
{
	if(entry == NULL) return;
	memcpy(entry->filename, filename, 11);
	entry->num_sectors = num_sectors;
	entry->start = start;
}
/* Print binary table from entry.
 */
void print_entry(struct file *entry)
{
	if(entry == NULL) return;
	printf("================ Entry [%s] ======================\n",
		entry->filename);
	write(STDOUT_FILENO, entry, sizeof(struct file));
	printf("\n=================================================\n");
}
/* Write to file buffer with limit.
 */
int write_file(int fout, unsigned char sector_skip,
	unsigned char sector_count, const char *filename)
{
	int nbytes, total_bytes = 0;
	int total_sectors = 0;
	char buf[512];
	int fin;

	errno = 0;
	if((fin = open(filename, O_RDONLY)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return 1;
	}
	/* TODO: Take care of error handling. */
	lseek(fout, sector_skip*512, SEEK_SET);
	while(total_sectors < sector_count
			&& (nbytes = read(fin, buf, sizeof(buf))) > 0) {
		if((nbytes = write(fout, buf, nbytes)) >= 0) {
			total_bytes += nbytes;
			total_sectors++;
		}
	}
	close(fin);
	printf("Total bytes written: %d\n", total_bytes);
	printf("Total sectors written: %d/%d\n", total_sectors, sector_count);
	return 0;
}
/* Write file table to disk.
 */
int write_table(const char *filename, struct file *table)
{
	int total_bytes = 0;
	int fd;

	errno = 0;
	if((fd = open(filename, O_WRONLY | O_CREAT, 0644)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return 1;
	}
	total_bytes = write(fd, table, sizeof(struct file)*MAXFILES);
	close(fd);
	printf("File %s created totaling %d bytes.\n", filename, total_bytes);
	printf("Size of table struct is %lu.\n", sizeof(struct file));
	return 0;
}

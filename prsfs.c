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

/* Number of files in file system */
static int _prsfs_file_count;
/* Initialise ftable memory.
 */
void init_table(struct file *table)
{
	const unsigned char c = 0xf7;
	int i;
	memset(table, 0, sizeof(struct file)*MAXFILES);
	for(i = 0; i < MAXFILES; i++)
		memcpy(table[i].filename, &c, 1);
	_prsfs_file_count = 0;
}
/* Initialise ftable entry.
 */
void init_entry(struct file *entry, const char *filename)
{
	if(entry == NULL) return;
	memcpy(entry->filename, filename, 11);
	entry->num_sectors = 0;
	entry->start = 1;
	entry->_reserved = 0;
	_prsfs_file_count++;
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
		if((nbytes = write(fout, buf, nbytes)) > 0) {
			total_bytes += nbytes;
			total_sectors++;
		}
	}
	close(fin);
	printf("Total bytes written: %d\n", total_bytes);
	printf("Total sectors written: %d/%d\n", total_sectors, sector_count);
	return 0;
}
/* Get filename from table entry.
 */
char *get_filename(struct file *entry)
{
	static char filename[13];
	int i, j, k;

	for(i = 0, k = 0; k < 8 && entry->filename[i] != ' '; i++, k++)
		filename[i] = (char)entry->filename[k];
	while(k < 8 && entry->filename[k] == ' ') k++;
	if(entry->extension[0] != ' ')
		filename[i++] = '.';
	for(j = 0; entry->extension[i] != ' ' && j < 3; j++, i++)
		filename[i] = (char)entry->extension[j];
	filename[i] = '\0';
	return filename;
}
/* Write file table to disk.
 */
int write_table(int fout, struct file *table, unsigned short start)
{
	int total_bytes = 0;

	if(_prsfs_file_count > 0) {
		int i;

		for(i = 0; i < _prsfs_file_count; i++) {
			const char *filename = get_filename(&table[i]);
			struct stat st;
			int fd;

			if((fd = open(filename, O_RDONLY, S_IRUSR | S_IRGRP)) < 0) continue;
			if(fstat(fd, &st) < 0) {
				fprintf(stderr, "Warning: Could not stat() file: %s\n",
					filename);
				close(fd);
				continue;
			}
			close(fd);
			table[i].start = start;
			table[i].num_sectors = st.st_size/512;
			start += (st.st_size/512)+1;
		}
		errno = 0;
		total_bytes = write(fout, table, sizeof(struct file)*MAXFILES);
		if(errno != 0) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return 1;
		}
		printf("Table written totaling %d bytes.\n", total_bytes);
		printf("Size of table struct is %lu.\n", sizeof(struct file));
		for(i = 0; i < _prsfs_file_count; i++) {
			const char *filename = get_filename(&table[i]);
			printf("File: %s\n", filename);
			write_file(fout, table[i].start, table[i].num_sectors, filename);
			printf("Wrote %u sectors starting at %u\n",
				table[i].num_sectors, table[i].start);
		}
	}
	return 0;
}

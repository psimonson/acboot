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

/* Maximum files that can be stored */
#define MAXFILES 32
/* File table structure */
struct file {
	unsigned char id;
	unsigned short num_sectors;
	unsigned short start;
	unsigned char _unused[3];
	unsigned int _reserved;
};
/* Initialise ftable memory.
 */
void init_table(struct file *table)
{
	memset(table, 0, sizeof(struct file)*MAXFILES);
}
/* Initialise ftable entry.
 */
void init_entry(struct file *entry, unsigned char id,
	unsigned char num_sectors, unsigned char start)
{
	if(entry == NULL) return;
	entry->id = id;
	entry->num_sectors = num_sectors;
	entry->start = start;
}
/* Print binary table from entry.
 */
void print_entry(struct file *entry)
{
	if(entry == NULL) return;
	printf("================ Entry #%d ======================\n", entry->id);
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
		nbytes = write(fout, buf, nbytes);
		total_bytes += nbytes;
		if((total_bytes % 512) == 0) total_sectors++;
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
/* Program to create a simple file system.
 */
int main(void)
{
	struct file ftable[MAXFILES];
	int fout;

	init_table(ftable);
	init_entry(&ftable[0], 1, 1, 0);
	init_entry(&ftable[1], 2, 1, 1);
	init_entry(&ftable[2], 3, 5, 2);
	errno = 0;
	if((fout = open("floppy.img", O_RDWR | O_CREAT)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return 1;
	}
	write_file(fout, 0, 1, "boot.bin");
	write_table("ftable.dat", ftable);
	write_file(fout, 1, 1, "ftable.dat");
	write_file(fout, 2, 5, "io.sys");
	close(fout);
	return 0;
}

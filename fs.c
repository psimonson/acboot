/**
 * @file fs.c
 * @author Philip R. Simonson
 * @date 02/15/2020
 * @brief Simple custom file system handling functions.
 ********************************************************************
 */

asm(".code16gcc");

#include "stdio.h"
#include "disk.h"
#include "fs.h"

#define SHELL_ENTRY 0x1000

/* Get file name from entry.
 */
char *get_filename(const struct file *entry)
{
	static char filename[13];
	int i, j, k;

	memset(filename, 0, 13);
	for(i = 0, k = 0; k < 8 && entry->filename[k] != ' '; i++, k++)
		filename[i] = (char)entry->filename[k];
	while(k < 8 && entry->filename[k] == ' ') k++;
	if(entry->extension[0] != ' ')
		filename[i++] = '.';
	for(j = 0; entry->extension[i] != ' ' && j < 3; j++, i++)
		filename[i] = (char)entry->extension[j];
	filename[i] = '\0';
	return filename;
}
/* Get file name from user.
 */
char *get_filename_user(const char *filename)
{
	static char converted[13];
	int i, j, len = strlen(filename);

	memset(converted, 0, 13);
	for(i = 0, j = 0;
		j < 8 && filename[j] != '.'; i++,j++)
		converted[i] = filename[j];
	while(i < 8 && filename[j] == '.')
		converted[i++] = ' ';
	++j;
	for(; i <= 11 || (j <= len && filename[j] != ' '); i++,j++)
		converted[i] = filename[j];
	converted[i] = '\0';
	return converted;
}
/* Search for file in file system and return if found.
 */
struct file *search_file(const unsigned char *ftable, const char *filename)
{
	struct file *entry;
	int i;

	for(i = 0; i < MAXFILES; i++) {
		entry = (struct file *)&ftable[i*16];
		if(entry->filename[0] != 0xf7 || entry->filename[0] != 0x00) {
			if(memcmp(filename, entry->filename, 11) == 0)
				return entry;
		}
	}
	return NULL;
}
/* Execute file entry and jump to it.
 */
void exec_file(const drive_params_t *p, const struct file *entry)
{
	void *e = (void*)SHELL_ENTRY;
	unsigned short start_sector = entry->start;
	unsigned short num_sectors = entry->num_sectors;
	int num_read = 0;

	reset_disk(p);
	if((num_read = read_disk(e, p, start_sector, num_sectors))
			== num_sectors) {
		asm volatile(
			"movb %0, %%dl\n"
			"pusha\n"
			"call $0x1000, $0x0000\n"
			"popa\n"
			: : "r"(p->drive)
		);
	}

	printf("Binary could not be loaded.\r\n"
		"Total sectors read: %d/%d\r\n", num_read, num_sectors);
}
/* List root directory (all files).
 */
void list_files(const drive_params_t *p)
{
	unsigned char *table = get_ftable(p);
	if(table != NULL) {
		int i, count = 0;

		for(i = 0; i < MAXFILES; i++) {
			struct file *entry = (struct file *)&table[i*16];
			if(entry->filename[0] == 0xf7) continue;
			printf("%s [Sectors occuped: %d, Starting sector: %d]\r\n",
				get_filename(entry), entry->num_sectors, entry->start);
			count++;
		}
		printf("Total files in root: %d\r\n", count);
		return;
	}
	printf("No files on the current disk.\r\n");
}

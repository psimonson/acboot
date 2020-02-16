/**
 * @file fs.c
 * @author Philip R. Simonson
 * @date 02/15/2020
 * @brief Simple custom file system handling functions.
 ********************************************************************
 */

asm(".code16gcc\n");

#include "stdio.h"
#include "disk.h"
#include "fs.h"

#define IMAGE_ENTRY 0x8000
/* Get file name from entry.
 */
char *get_filename(const struct file *entry)
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
/* Search for file in file system and return if found.
 */
struct file *search_file(const unsigned char *ftable, const char *filename)
{
	struct file *entry;
	int i;

	for(i = 0; i < MAXFILES; i++) {
		entry = (struct file *)&ftable[i*16];
		if(memcmp(entry->filename, filename, 11) == 0)
			return entry;
	}
	return NULL;
}
/* Execute file entry and jump to it.
 */
void exec_file(const drive_params_t *p, const struct file *entry)
{
	void *buffer = (void*)IMAGE_ENTRY;
	void *e = (void*)IMAGE_ENTRY;
	reset_disk(p);
	if(read_disk(buffer, p, entry->start, entry->num_sectors)
			== entry->num_sectors) {
		asm volatile("" : : "d"(p->drive));
		goto *e;
	}
	printf("Binary could not be loaded.\r\n");
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

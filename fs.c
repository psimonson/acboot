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

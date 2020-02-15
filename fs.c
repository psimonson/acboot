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

/* Search for file in file system and return if found.
 */
struct file *search_file(const unsigned char *ftable, const char *filename)
{
	struct file *entry;
	int i;

	for(i = 0; i < MAXFILES; i++) {
		entry = (struct file *)&ftable[i*16];
		if(memcpy(entry->filename, filename, 11) == 0)
			return entry;
	}
	return NULL;
}

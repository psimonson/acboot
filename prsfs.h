/**
 * @file prsfs.h
 * @author Philip R. Simonson
 * @date 02/11/2020
 * @brief Simple file system header for floppy.
 *************************************************************
 */

#ifndef _PRSFS_H_
#define _PRSFS_H_

/* my file system defines and structure */
#include "types.h"

void init_table(struct file *table);
void init_entry(struct file *entry, const char *filename,
	unsigned short num_sectors, unsigned short start);
void print_entry(struct file *entry);
int write_file(int fout, unsigned char sector_skip,
	unsigned char sector_count, const char *filename);
int write_table(int fout, struct file *table);
unsigned char *get_entry(unsigned char drive);

#endif

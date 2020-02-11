/**
 * @file prsfs.h
 * @author Philip R. Simonson
 * @date 02/11/2020
 * @brief Simple file system header for floppy.
 *************************************************************
 */

#ifndef _PRSFS_H_
#define _PRSFS_H_

/* Maximum files that can be stored */
#define MAXFILES 32
/* File table structure */
struct file {
	unsigned char filename[11];
	unsigned short num_sectors;
	unsigned short start;
	unsigned char _reserved;
} __attribute((packed));
void init_table(struct file *table);
void init_entry(struct file *entry, const char *filename,
	unsigned short num_sectors, unsigned short start);
void print_entry(struct file *entry);
int write_file(int fout, unsigned char sector_skip,
	unsigned char sector_count, const char *filename);
int write_table(const char *filename, struct file *table);

#endif

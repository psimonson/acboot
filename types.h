/**
 * @file types.h
 * @author Philip R. Simonson
 * @date 02/11/2020
 * @brief Simple file system types header for floppy.
 *************************************************************
 */

#ifndef _TYPES_H_
#define _TYPES_H_

/* Maximum files that can be stored */
#define MAXFILES 32
/* File table structure */
struct file {
	unsigned char filename[8];
	unsigned char extension[3];
	unsigned char _reserved;
	unsigned short count;
	unsigned short start;
} __attribute((packed));

#endif

/**
 * @file disk.h
 * @author Philip R. Simonson
 * @date 02/14/2020
 * @brief Simple disk I/O (read/write).
 ******************************************************************
 */

#ifndef _DISK_H_
#define _DISK_H_

#include "attr.h"

#define NULL ((void*)0)
#define BLOCK_SIZE 512

/* drive params structure */
struct drive_params {
	unsigned char spt;
	unsigned char numh;
	unsigned char drive;
} __PACKED;
typedef struct drive_params drive_params_t;
/* get drive parameters from BIOS */
__REGPARM int get_drive_params(const unsigned char drive, drive_params_t *p);
/* reset disk drive */
__REGPARM int reset_disk(const drive_params_t *p);
/* raw read drive from LBA */
__REGPARM int read_disk(const void *buffer, const drive_params_t *p,
	unsigned int lba, unsigned char blocks);
/* raw write drive from LBA */
__REGPARM int write_disk(const void *buffer, const drive_params_t *p,
	unsigned int lba, unsigned char blocks);
/* read my file system table from disk */
__REGPARM void *get_ftable(const drive_params_t *p);
/* display table contents in hexadecimal */
__REGPARM void dump_table(const unsigned char *table);

#endif

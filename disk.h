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
int __REGPARM get_drive_params(const unsigned char drive, drive_params_t *p);
/* raw read drive from LBA */
int __REGPARM read_disk(const void *buffer, const drive_params_t *p,
	unsigned int lba, unsigned char blocks);
/* raw write drive from LBA */
void __REGPARM write_disk(const unsigned char drive);

#endif

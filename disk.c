/**
 * @file disk.c
 * @author Philip R. Simonson
 * @date 02/14/2020
 * @brief Simple implementation of disk I/O (read/write).
 ********************************************************************
 */

asm(".code16gcc");

#include "stdio.h"
#include "disk.h"

/* Get drive parameters from BIOS.
 */
__REGPARM int get_drive_params(const unsigned char drive, drive_params_t *p)
{
	unsigned char failed = 0;
	unsigned short tmp1, tmp2;

	asm volatile(
		"push %%ds\n"
		"push %%es\n"
		"int $0x13\n"
		"setcb %0\n"
		"pop %%es\n"
		"pop %%ds\n"
		: "=r"(failed), "=c"(tmp1), "=d"(tmp2)
		: "a"(0x0800), "d"(0x0000 | drive), "D"(0)
		: "cc", "bx"
	);

	if(failed)
		return failed;
	p->drive = drive;
	p->spt = tmp1 & 0x3f;
	p->numh = tmp2 >> 8;
	return failed;
}
/* Reset disk drive.
 */
__REGPARM int reset_disk(const drive_params_t *p)
{
	unsigned char failed = 0;
	asm volatile(
		"int $0x13\n"
		"setcb %0\n"
		: "=r"(failed)
		: "a"(0x0000), "d"(0x0000 | p->drive)
		: "cc"
	);
	return !failed;
}
/* Raw read from disk drive.
 */
__REGPARM int read_disk(const void *buffer, const drive_params_t *p,
	unsigned int lba, unsigned char blocks)
{
	unsigned short status = 0;
	unsigned char failed = 0;
	unsigned char c, h, s;

	/* LBA to CHS value */
	s = (lba % p->spt) + 1;
	c = (lba / p->spt) / p->numh;
	h = (lba / p->spt) % p->numh;

	asm volatile(
		"int $0x13\n"
		"setcb %0\n"
		: "=r"(failed), "=a"(status)
		: "a"(0x0200 | blocks), "b"(buffer), "c"((c << 8) | s),
			"d"((h << 8) | p->drive)
	);

	if(failed)
		return 0;
	return (status & 0x00ff);
}
/* Raw write to disk drive.
 */
__REGPARM int write_disk(const void *buffer, const drive_params_t *p,
	unsigned int lba, unsigned char blocks)
{
	unsigned short status = 0;
	unsigned char failed = 0;
	unsigned char c, h, s;

	/* convert lba to chs */
	s = (lba % p->spt) + 1;
	c = (lba / p->spt) / p->numh;
	h = (lba / p->spt) % p->numh;

	asm volatile(
		"int $0x13\n"
		"setcb %0\n"
		: "=r"(failed), "=a"(status)
		: "a"(0x0300 | blocks), "b"(buffer), "c"((c << 8) | s),
			"d"((h << 8) | p->drive)
	);

	if(failed)
		return 0;
	return (status & 0x00ff);
}
/* Get my file system table from disk.
 */
__REGPARM void *get_ftable(const drive_params_t *p)
{
	static unsigned char sector[BLOCK_SIZE];
	static char loaded = 0;

	if(loaded == 0) {
		reset_disk(p);
		if(read_disk(sector, p, 1, 1) == 1) {
			loaded = 1;
			return sector;
		} else {
			loaded = 0;
			return NULL;
		}
	}
	return sector;
}
/* Dump table to standard output.
 */
__REGPARM void dump_table(const unsigned char *table)
{
	int i, j;
	for(i = 0, j = 0; i < BLOCK_SIZE; i++) {
		if((i % 26) == 0) {
			j++;
			printf("\r\n");
		} else {
			printf("%x ", table[i]);
		}
		if(j > 0 && (j % 20) == 0) {
			printf("Press a key to continue...\r\n");
			(void)getc();
		}
	}
	printf("\r\n");
}

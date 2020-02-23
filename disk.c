/**
 * @file disk.c
 * @author Philip R. Simonson
 * @date 02/14/2020
 * @brief Simple implementation of disk I/O (read/write).
 ********************************************************************
 */

asm(".code16gcc");

#include "disk.h"

static unsigned char _file_table[BLOCK_SIZE];

/* Get drive parameters from BIOS.
 */
__REGPARM int get_drive_params(const unsigned char drive, drive_params_t *p)
{
	unsigned char failed = 0;
	unsigned short tmp1, tmp2;

	asm volatile(
		"push %%ds\n"
		"push %%es\n"
		"mov $0, %0\n"
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
		"push %%ds\n"
		"push %%es\n"
		"movb $0, %0\n"
		"int $0x13\n"
		"setcb %0\n"
		"pop %%es\n"
		"pop %%ds\n"
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
	lba_chs(p, lba, &c, &h, &s);

	asm volatile(
		"push %%ds\n"
		"push %%es\n"
		"movw %[buf], %%es\n"
		"movb $0, %0\n"
		"int $0x13\n"
		"setcb %0\n"
		"pop %%es\n"
		"pop %%ds\n"
		: "=r"(failed), "=a"(status)
		: "a"(0x0200 | blocks), [buf]"r"(buffer), "c"((c << 8) | s),
			"d"((h << 8) | p->drive), "b"(0x0000)
		: "cc"
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

	/* LBA to CHS value */
	lba_chs(p, lba, &c, &h, &s);

	asm volatile(
		"push %%ds\n"
		"push %%es\n"
		"movw %[buf], %%es\n"
		"movb $0, %0\n"
		"int $0x13\n"
		"setcb %0\n"
		"pop %%es\n"
		"pop %%ds\n"
		: "=r"(failed), "=a"(status)
		: "a"(0x0300 | blocks), [buf]"r"(buffer), "c"((c << 8) | s),
			"d"((h << 8) | p->drive), "b"(0x0000)
		: "cc"
	);

	if(failed)
		return 0;
	return (status & 0x00ff);
}
/* Load my file system table from disk.
 */
void *load_table(const drive_params_t *p)
{
	reset_disk(p);
	if(read_disk(_file_table, p, 1, 1) == 1)
		return _file_table;
	return NULL;
}
/* Get my file system table from memory.
 */
void *get_table(void)
{
	return _file_table;
}
/* Convert Logical block addressing to CHS.
 */
void lba_chs(const drive_params_t *p, unsigned int lba,
	unsigned char *c, unsigned char *h, unsigned char *s)
{
	unsigned int temp = lba / p->spt;
	*c = temp / p->numh;
	*h = temp % p->numh;
	*s = (lba % p->spt) + 1;
}

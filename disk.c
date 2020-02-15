/**
 * @file disk.c
 * @author Philip R. Simonson
 * @date 02/14/2020
 * @brief Simple implementation of disk I/O (read/write).
 ********************************************************************
 */

asm(".code16gcc");

#include "disk.h"

/* Get drive parameters from BIOS.
 */
int __REGPARM get_drive_params(const unsigned char drive, drive_params_t *p)
{
	unsigned char failed = 0;
	unsigned short tmp1, tmp2;

	asm volatile(
		"push %%bx\n"
		"push %%es\n"
		"mov $0, %0\n"
		"int $0x13\n"
		"setcb %0\n"
		"pop %%es\n"
		"pop %%bx\n"
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
/* Raw read from disk drive.
 */
int __REGPARM read_disk(const void *buffer, const drive_params_t *p,
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
		"push %%bx\n"
		"push %%es\n"
		"movb $0, %0\n"
		"int $0x13\n"
		"setcb %0\n"
		"pop %%es\n"
		"pop %%bx\n"
		: "=r"(failed), "=a"(status)
		: "a"(0x0200 | blocks), "b"(buffer), "c"((c << 8) | s),
			"d"((h << 8) | p->drive)
		: "cc"
	);

	if(failed)
		return 0;
	return (status & 0x00ff);
}
/* Raw write to disk drive.
 */
void __REGPARM write_disk(const unsigned char drive)
{
	/* TODO: Implement raw write. */
}

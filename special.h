/**
 * @file special.h
 * @author Philip R. Simonson
 * @date 02/26/2020
 * @brief Special functions header.
 *************************************************************
 */

#ifndef _SPECIAL_H_
#define _SPECIAL_H_

/* ---------------------- Inline ASM Functions ---------------------- */

__REGPARM void draw_pixel(unsigned char color, unsigned short x,
	unsigned short y);
__REGPARM int kbhit(void);

/* --------------------------- C Functions -------------------------- */

void beep(unsigned short freq, unsigned short high, unsigned short low);
void _type(const char *s, unsigned short freq, unsigned short high,
	unsigned short low);
#define type(m) _type(m, 4560, 0x0005, 0x05a0)

#endif

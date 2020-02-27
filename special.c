/**
 * @file special.c
 * @author Philip R. Simonson
 * @date 02/26/2020
 * @brief Special functions by me.
 *************************************************************
 */

asm(".code16gcc");

#include "stdio.h"
#include "system.h"

/* Draw a pixel on the screen.
 */
__REGPARM void draw_pixel(unsigned char color, unsigned short x,
	unsigned short y) {
	asm volatile(
		"int $0x10"
		: : "a"(0x0c00 | color), "c"(x), "d"(y)
	);
}
/* Check for keypress from keyboard.
 */
__REGPARM int kbhit(void)
{
	unsigned char ch = 0;
	asm volatile(
		"int $0x16; mov %%al, %0" : "=m"(ch) : "a"(0x0100)
	);
	return ch;
}

/* ----------------------- C Only Functions ----------------------- */

/* Beep at a certain frequency for a specified amount of time.
 */
void beep(unsigned short freq, unsigned short high, unsigned short low)
{
	outb(0x43, 182);
	outb(0x42, (unsigned char)(freq));
	outb(0x42, (unsigned char)(freq >> 8));
	outb(0x61, inb(0x61) | 0x03);
	timer(high, low);
	outb(0x61, inb(0x61) & 0x3c);
	timer(high, low);
}
/* Print text like a typer writer.
 */
void _type(const char *s, unsigned short freq, unsigned short high,
	unsigned short low)
{
	while(*s) {
		putc(*s++);
		if(!isspace(*s))
			beep(4560, 3, 0x0a20);
	}
}

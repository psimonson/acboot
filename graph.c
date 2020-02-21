/**
 * @file graph.c
 * @author Philip R. Simonson
 * @date 02/17/2020
 * @brief Simple graphics demonstration.
 ***********************************************************************
 */

asm(".code16gcc");
asm("jmp main");

#include "stdio.h"
#include "types.h"
#include "disk.h"

#define MAX_COLS	320
#define MAX_ROWS	200

#define SHELL_ENTRY 0x7e00

/* Program to display rectangles on screen.
 */
void main(void)
{
	const void *e = (const void *)SHELL_ENTRY;
	unsigned char drive = -1;
	drive_params_t p;
	unsigned short i = 0, j = 0, m = 0;
	unsigned short cnt1 = 0, cnt2 = 0;
	unsigned color = 10;

	setup();
	asm volatile("" : "=d"(drive));
	get_drive_params(drive, &p);
	clrscr(0x03);
	printf("This is a graphics demonstration it waits until a\r\n"
		"key is pressed otherwise continues to draw rectangles.\r\n"
		"Press any key to continue to the demonstration...\r\n");
	(void)getc();
	clrscr(0x13);
	while(!kbhit()) {
		if(m < (MAX_ROWS - m))
			++cnt1;
		if(m < (MAX_COLS - m - 3))
			++cnt2;

		if(cnt1 != cnt2) {
			cnt1 = 0;
			cnt2 = 0;
			m = 0;
			if(++color > 255) color = 0;
		}

		/* (left, top) to (left, bottom) */
		j = 0;
		for(i = m; i < MAX_ROWS - m; ++i)
			draw_pixel(color, j + m, i);
		/* (left, bottom) to (right, bottom) */
		for(j = m; j < MAX_COLS - m; ++j)
			draw_pixel(color, j, i);
		/* (right, bottom) to (right, top) */
		for(i = MAX_ROWS-m-1; i >= m; --i)
			draw_pixel(color, MAX_COLS - m - 1, i);
		/* (right, top) to (left, top) */
		for(j = MAX_COLS-m-1; j >= m; --j)
			draw_pixel(color, j, m);

		m += 6;
		if(++color > 255) color = 0;
	}
	clrscr(0x03);
	asm("" : : "d"(p.drive));
	goto *e;
}

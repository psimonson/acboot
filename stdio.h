/**
 * @file stdio.c
 * @author Philip R. Simonson
 * @date 02/07/2020
 * @brief Simple I/O implementation header.
 *************************************************************
 */

#ifndef _STDIO_H_
#define _STDIO_H_

#include "attr.h"

__REGPARM void timer(unsigned short high, unsigned short low);
__REGPARM void outb(unsigned short port, unsigned char val);
__REGPARM unsigned char inb(unsigned short port);
__REGPARM int getc(void);
__REGPARM void putc(int c);
int isspace(int c);
void beep(unsigned short freq, unsigned short high, unsigned short low);
void _type(const char *s, unsigned short freq, unsigned short high,
	unsigned short low);
#define type(m) _type(m, 4560, 0x0005, 0x05a0)
int strlen(const char *s);
void reverse(char *s);
void itoa(int n, char *s, int size);
void print(const char *data, int len);
int printf(const char *fmt, ...);
void gets(char *s, int size);

#endif

/**
 * @file stdio.c
 * @author Philip R. Simonson
 * @date 02/07/2020
 * @brief Simple I/O implementation by me.
 *************************************************************
 */

asm(".code16gcc");

#include <stddef.h>
#include <stdarg.h>
#include "stdio.h"

/* Setup segment registers.
 */
__REGPARM void setup(void)
{
	asm volatile(
		"xorw %ax, %ax\n"
		"movw %ax, %ds\n"
		"movw %ax, %es\n"
		"movw %ax, %fs\n"
		"movw %ax, %gs\n"
	);
}
/* Get cursor position on screen.
 */
__REGPARM void getcur(unsigned char *x, unsigned char *y)
{
	unsigned short pos = 0;
	asm volatile(
		"int $0x10"
		: "=d"(pos)
		: "a"(0x0300), "b"(0x0000)
	);
	*x = (unsigned char)(pos);
	*y = (unsigned char)(pos >> 8);
}
/* Move cursor position on screen.
 */
__REGPARM void mvcur(unsigned char x, unsigned char y)
{
	asm volatile(
		"int $0x10"
		: : "a"(0x0200), "b"(0x0007), "d"((y << 8) | x)
	);
}
/* Clear the screen.
 */
__REGPARM void clrscr(unsigned char mode)
{
	asm volatile(
		"int $0x10"
		: : "a"(0x0000 | mode)
	);
}
/* Get character from keyboard.
 */
__REGPARM int getc(void)
{
	unsigned char ch = 0;
	asm("int $0x16\nmov %%al, %0" : "=m"(ch) : "a"(0x0000));
	return ch;
}
/* Put character to screen.
 */
__REGPARM void putc(int c)
{
	asm volatile(
		"int $0x10" : : "a"(0x0e00 | (c & 0x000000ff)),
		"b"(0x0007), "c"(0x0001)
	);
}

/* ----------------------- C Only Functions ----------------------- */

/* Check if character is whitespace.
 */
int isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r'
		|| c == '\n');
}
/* String length function.
 */
int strlen(const char *s)
{
	int i;
	for(i = 0; *s != '\0'; s++, i++);
	return i;
}
/* Reverse string in place.
 */
void reverse(char *s)
{
	char *p, *t;
	for(t = s; *t != '\0'; t++);
	t--;
	for(p = s; *p != '\0' && *p != *t; p++,t--) {
		char c = *s;
		*s = *t;
		*t = c;
	}
}
/* Convert hexidecimal number to string.
 */
void itoh(int n, char *s, int size)
{
	const char *digits = "0123456789ABCDEF";
	char *p = s;

	if(n < 0)
		n = -n;
	do {
		*p++ = digits[n%16];
	} while((n /= 16) > 0);
	*p = '\0';
}
/* Convert decimal number to string.
 */
void itoa(int n, char *s, int size)
{
	char *p = s;
	int sign;

	if((sign = n) < 0)
		n = -n;
	do {
		*p++ = n % 10 + '0';
	} while(--size > 0 && (n /= 10) > 0);
	if(sign < 0)
		*p++ = '-';
	*p = '\0';
	reverse(s);
}
/* This function is used for printf.
 */
void print(const char *data, int len)
{
	const unsigned char *bytes = (const unsigned char*)data;
	int i;
	for(i = 0; i < len; i++)
		putc(bytes[i]);
}
/* Print formatted output to screen.
 */
int printf(const char *fmt, ...)
{
	int written;
	va_list ap;
	written = 0;
	va_start(ap, fmt);
	while(*fmt != '\0') {
		const char *p = NULL;

		if(fmt[0] != '%' || fmt[1] == '%') {
			int amount = 1;
			if(fmt[0] == '%')
				fmt++;
			while(fmt[amount] && fmt[amount] != '%')
				amount++;
			print(fmt, amount);
			fmt += amount;
			written += amount;
			continue;
		}
		p = fmt++;
		if(*fmt == 'c') {
			char c = (char)va_arg(ap, int);
			fmt++;
			print(&c, sizeof(c));
			written++;
		} else if(*fmt == 'd') {
			char buf[32];	/* buffer to hold string */
			int len;		/* length of buf */
			int n = va_arg(ap, int);
			fmt++;
			itoa(n, buf, sizeof(buf));
			len = strlen(buf);
			print(buf, len);
			written += len;
		} else if(*fmt == 's') {
			const char *str = va_arg(ap, const char *);
			int len = strlen(str);
			fmt++;
			print(str, len);
			written += len;
		} else if(*fmt == 'x') {
			char buf[32];	/* buffer to hold string */
			int len;		/* length of buf */
			int n = va_arg(ap, int);
			fmt++;
			itoh(n, buf, sizeof(buf));
			len = strlen(buf);
			print(buf, len);
			written += len;
		} else {
			int len = strlen(fmt);
			fmt = p;
			print(fmt, len);
			written += len;
			fmt += len;
		}
	}
	va_end(ap);
	return written;
}
/* Get input from user into a buffer.
 */
int gets(char s[], int size)
{
	unsigned char x, y;
	int c, i;

	for(i = 0; i < size && (c = getc()) != '\r'; ) {
		if(c == '\b') {
			if(i <= 0) i = 0;
			else {
				s[--i] = '\0';
				getcur(&x, &y);
				mvcur(x-1, y);
				putc(' ');
				mvcur(x-1, y);
			}
		} else {
			s[i++] = c;
			putc(c);
		}
	}
	printf("\r\n");
	s[i] = '\0';
	return i;
}
/* Compare memory against each other to see if they're equal.
 */
int memcmp(const void *s, const void *t, int size)
{
	const unsigned char *p1, *p2;
	int i;

	p1 = (const unsigned char *)s;
	p2 = (const unsigned char *)t;
	for(i = 0; i < size; i++) {
		if(p1[i] < p2[i])
			return -1;
		else if(p1[i] > p2[i])
			return 1;
	}
	return 0;
}
/* Set memory block to a value.
 */
void *memset(void *p, int value, int size)
{
	unsigned char *buf = (unsigned char *)p;
	int i;
	for(i = 0; i < size; i++)
		buf[i] = (unsigned char)value;
	return p;
}
/* Copy memory from one place to another.
 */
void *memcpy(void *dstp, const void *srcp, int size)
{
	unsigned char *dst = (unsigned char *)dstp;
	const unsigned char *src = (const unsigned char *)srcp;
	int i;
	for(i = 0; i < size; i++)
		dst[i] = src[i];
	return dstp;
}
/* Compare two strings together.
 */
int strcmp(const char *s, const char *t)
{
	return memcmp(s, t, strlen(t));
}
